pub trait LazyTag: Copy + Default {
    fn apply(&mut self, t: Self);
    fn is_empty(&self) -> bool;
}

pub trait SegmentInfo<T: LazyTag>: Copy + Default {
    fn merge(left: Self, right: Self) -> Self;
    fn apply(&mut self, t: T, len: usize);
}

#[derive(Clone, Copy, Default, Debug)]
pub struct Tag {
    pub add: i64,
}

impl LazyTag for Tag {
    fn apply(&mut self, t: Self) {
        self.add += t.add;
    }

    fn is_empty(&self) -> bool {
        self.add == 0
    }
}

impl SegmentInfo<Tag> for i64 {
    fn merge(left: Self, right: Self) -> Self {
        left + right
    }

    fn apply(&mut self, t: Tag, len: usize) {
        *self += len as i64 * t.add;
    }
}

pub struct SegTree<I, T> {
    n:    usize,
    info: Vec<I>,
    lazy: Vec<T>,
}

impl<I, T> SegTree<I, T>
where
    I: SegmentInfo<T>,
    T: LazyTag,
{
    pub fn new(a: &[I]) -> Self {
        let mut seg = Self::with_len(a.len());
        seg.build(a);
        seg
    }

    pub fn with_len(n: usize) -> Self {
        assert!(n > 0);
        let size = 4usize << n.ilog2();
        Self {
            n:    n,
            info: vec![I::default(); size],
            lazy: vec![T::default(); size],
        }
    }

    pub fn build(&mut self, a: &[I]) {
        assert_eq!(self.n, a.len());
        self.lazy.fill(T::default());
        self._build(1, 0, a.len() - 1, a);
    }

    fn _build(&mut self, o: usize, l: usize, r: usize, a: &[I]) {
        if l == r {
            self.info[o] = a[l];
            return;
        }
        let m = l + (r - l) / 2;
        self._build(o << 1, l, m, a);
        self._build(o << 1 | 1, m + 1, r, a);
        self.pull(o);
    }

    fn pull(&mut self, o: usize) {
        self.info[o] = I::merge(self.info[o << 1], self.info[o << 1 | 1]);
    }

    fn apply(&mut self, o: usize, l: usize, r: usize, t: T) {
        let len = r - l + 1;
        self.info[o].apply(t, len);
        self.lazy[o].apply(t);
    }

    fn push(&mut self, o: usize, l: usize, r: usize) {
        if l == r || self.lazy[o].is_empty() {
            return;
        }
        let m = l + (r - l) / 2;
        let t = self.lazy[o];
        self.apply(o << 1, l, m, t);
        self.apply(o << 1 | 1, m + 1, r, t);
        self.lazy[o] = T::default();
    }
}

impl<I, T> SegTree<I, T>
where
    I: SegmentInfo<T>,
    T: LazyTag,
{
    pub fn modify_point(&mut self, p: usize, val: I) {
        assert!(p < self.n);
        self._modify_point(p, val, 1, 0, self.n - 1);
    }

    fn _modify_point(&mut self, p: usize, val: I, o: usize, l: usize, r: usize) {
        if l == r {
            self.info[o] = val;
            self.lazy[o] = T::default();
            return;
        }
        self.push(o, l, r);
        let m = l + (r - l) / 2;
        if p <= m {
            self._modify_point(p, val, o << 1, l, m);
        } else {
            self._modify_point(p, val, o << 1 | 1, m + 1, r);
        }
        self.pull(o);
    }

    pub fn modify_range(&mut self, ql: usize, qr: usize, t: T) {
        assert!(ql <= qr && qr < self.n);
        return self._modify_range(ql, qr, t, 1, 0, self.n - 1);
    }

    fn _modify_range(&mut self, ql: usize, qr: usize, t: T, o: usize, l: usize, r: usize) {
        if ql <= l && r <= qr {
            self.apply(o, l, r, t);
            return;
        }
        self.push(o, l, r);
        let m = l + (r - l) / 2;
        if ql <= m {
            self._modify_range(ql, qr, t, o << 1, l, m);
        }
        if qr > m {
            self._modify_range(ql, qr, t, o << 1 | 1, m + 1, r);
        }
        self.pull(o);
    }

    pub fn query_point(&mut self, p: usize) -> I {
        assert!(p < self.n);
        return self._query_point(p, 1, 0, self.n - 1);
    }

    fn _query_point(&mut self, p: usize, o: usize, l: usize, r: usize) -> I {
        if l == r {
            return self.info[o];
        }
        self.push(o, l, r);
        let m = l + (r - l) / 2;
        if p <= m {
            return self._query_point(p, o << 1, l, m);
        }
        return self._query_point(p, o << 1 | 1, m + 1, r);
    }

    pub fn range_query(&mut self, ql: usize, qr: usize) -> I {
        assert!(ql <= qr && qr < self.n);
        return self._range_query(ql, qr, 1, 0, self.n - 1);
    }

    fn _range_query(&mut self, ql: usize, qr: usize, o: usize, l: usize, r: usize) -> I {
        if ql <= l && r <= qr {
            return self.info[o];
        }
        self.push(o, l, r);
        let m = l + (r - l) / 2;
        if m >= qr {
            return self._range_query(ql, qr, o << 1, l, m);
        }
        if m < ql {
            return self._range_query(ql, qr, o << 1 | 1, m + 1, r);
        }
        return I::merge(
            self._range_query(ql, qr, o << 1, l, m),
            self._range_query(ql, qr, o << 1 | 1, m + 1, r),
        );
    }

    pub fn query_all(&self) -> I {
        return self.info[1];
    }

    pub fn to_vec(&mut self) -> Vec<I> {
        let mut res = Vec::with_capacity(self.n);
        self._collect(1, 0, self.n - 1, &mut res);
        res
    }

    fn _collect(&mut self, o: usize, l: usize, r: usize, res: &mut Vec<I>) {
        if l == r {
            res.push(self.info[o]);
            return;
        }
        self.push(o, l, r);
        let m = l + (r - l) / 2;
        self._collect(o << 1, l, m, res);
        self._collect(o << 1 | 1, m + 1, r, res);
    }
}
