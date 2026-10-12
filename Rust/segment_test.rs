#[path = "segment.rs"]
mod segment;

use self::segment::{LazyTag, SegTree, SegmentInfo, Tag};

type SumTree = SegTree<i64, Tag>;

// 期望结果只通过普通数组计算，不调用线段树的合并或标记方法。
fn assert_sum_ranges(seg: &mut SumTree, values: &[i64]) {
    assert_eq!(seg.query_all(), values.iter().sum::<i64>());
    for (p, &value) in values.iter().enumerate() {
        assert_eq!(seg.query_point(p), value, "point {p}");
        for r in p..values.len() {
            assert_eq!(
                seg.range_query(p, r),
                values[p..=r].iter().sum::<i64>(),
                "range [{p}, {r}]"
            );
        }
    }
    assert_eq!(seg.to_vec(), values);
}

#[test]
fn new_builds_all_inclusive_ranges_for_different_tree_shapes() {
    for n in [1, 2, 3, 4, 5, 7, 8, 9, 17, 31, 32, 33, 63, 64, 65, 127, 128, 129] {
        let values: Vec<i64> = (0..n).map(|i| i as i64 % 11 - 5).collect();
        assert_sum_ranges(&mut SumTree::new(&values), &values);
    }
}

#[test]
fn with_len_supports_sparse_assignments_without_build() {
    let mut seg = SumTree::with_len(7);
    assert_eq!(seg.query_all(), 0);
    assert_eq!(seg.range_query(1, 5), 0);
    assert_eq!(seg.to_vec(), vec![0; 7]);

    seg.modify_point(0, -3);
    seg.modify_point(3, 12);
    seg.modify_point(6, 5);
    assert_sum_ranges(&mut seg, &[-3, 0, 0, 12, 0, 0, 5]);
}

#[test]
fn point_modification_assigns_the_requested_value() {
    let mut seg = SumTree::new(&[2, 3, 5, 7]);
    seg.modify_point(2, 10);
    assert_eq!(seg.query_all(), 22);
    assert_eq!(seg.query_point(2), 10);
    seg.modify_point(2, -4);
    seg.modify_point(2, -4);
    assert_eq!(seg.query_all(), 8);
    seg.modify_point(2, 0);
    assert_sum_ranges(&mut seg, &[2, 3, 0, 7]);
}

#[test]
fn range_addition_includes_both_endpoints() {
    let mut seg = SumTree::new(&[1, 2, 3, 4, 5]);
    seg.modify_range(0, 0, Tag { add: 10 });
    seg.modify_range(4, 4, Tag { add: -10 });
    seg.modify_range(1, 3, Tag { add: -2 });
    assert_sum_ranges(&mut seg, &[11, 0, 1, 2, -5]);
}

#[test]
fn accumulated_full_range_tags_reach_partial_queries_once() {
    let mut seg = SumTree::new(&[2, 3, 5, 7, 11]);
    seg.modify_range(0, 4, Tag { add: 3 });
    seg.modify_range(0, 4, Tag { add: -1 });
    seg.modify_range(0, 4, Tag { add: 8 });
    // 先查看总和，不通过点查询或 to_vec 提前下传整棵树的标记。
    assert_eq!(seg.query_all(), 78);
    assert_eq!(seg.range_query(1, 3), 45);
    assert_eq!(seg.range_query(1, 3), 45);
    assert_sum_ranges(&mut seg, &[12, 13, 15, 17, 21]);
}

#[test]
fn point_assignment_overrides_pending_additions() {
    let mut seg = SumTree::new(&[2, 3, 5, 7, 11]);
    seg.modify_range(0, 4, Tag { add: 3 });
    seg.modify_range(1, 3, Tag { add: -2 });
    seg.modify_point(2, 100);
    assert_eq!(seg.query_all(), 131);
    seg.modify_range(2, 4, Tag { add: -5 });
    assert_sum_ranges(&mut seg, &[5, 4, 95, 3, 9]);
}

#[test]
fn opposite_additions_and_empty_tags_preserve_values() {
    let mut seg = SumTree::new(&[-5, 0, 8]);
    seg.modify_range(0, 2, Tag { add: 7 });
    seg.modify_range(0, 2, Tag { add: -7 });
    seg.modify_range(1, 2, Tag::default());
    assert_sum_ranges(&mut seg, &[-5, 0, 8]);
}

#[test]
fn rebuild_discards_old_pending_tags() {
    let mut seg = SumTree::new(&[1, 2, 3, 4, 5]);
    seg.modify_range(1, 3, Tag { add: 7 });
    seg.modify_range(0, 4, Tag { add: 100 });
    seg.build(&[-9, 4, -2, 0, 6]);
    assert_sum_ranges(&mut seg, &[-9, 4, -2, 0, 6]);
    seg.modify_range(0, 4, Tag { add: 3 });
    assert_sum_ranges(&mut seg, &[-6, 7, 1, 3, 9]);
}

#[test]
fn to_vec_is_repeatable_and_keeps_the_tree_usable() {
    let mut seg = SumTree::new(&[2, 3, 5, 7]);
    seg.modify_range(1, 2, Tag { add: -2 });
    seg.modify_range(0, 3, Tag { add: 10 });
    assert_eq!(seg.to_vec(), vec![12, 11, 13, 17]);
    assert_eq!(seg.to_vec(), vec![12, 11, 13, 17]);
    assert_eq!(seg.query_all(), 53);
    seg.modify_point(0, -5);
    seg.modify_range(2, 3, Tag { add: 2 });
    assert_sum_ranges(&mut seg, &[-5, 11, 15, 19]);
}

#[test]
fn singleton_supports_every_operation_and_rebuild() {
    let mut seg = SumTree::with_len(1);
    seg.modify_range(0, 0, Tag { add: -4 });
    assert_eq!(seg.query_point(0), -4);
    seg.modify_point(0, 7);
    seg.modify_range(0, 0, Tag { add: 2 });
    assert_sum_ranges(&mut seg, &[9]);
    seg.build(&[-3]);
    assert_sum_ranges(&mut seg, &[-3]);
}

// 固定种子，无外部随机数依赖。wrapping 保证 debug/release 下序列相同。
struct Rng(u64);

impl Rng {
    fn next(&mut self) -> u64 {
        self.0 = self.0.wrapping_mul(6364136223846793005).wrapping_add(1);
        self.0 >> 32
    }

    fn index(&mut self, n: usize) -> usize {
        (self.next() % n as u64) as usize
    }

    fn value(&mut self) -> i64 {
        (self.next() % 41) as i64 - 20
    }
}

#[test]
fn seeded_mixed_sum_operations_match_an_array() {
    for seed in [1, 42, 0x5eed] {
        for n in [1, 2, 3, 5, 8, 9, 17, 31, 32, 33, 63, 64, 65] {
            for zeroed in [false, true] {
                let mut rng = Rng(seed);
                let mut values: Vec<i64> = (0..n).map(|_| if zeroed { 0 } else { rng.value() }).collect();
                let mut seg = if zeroed {
                    SumTree::with_len(n)
                } else {
                    SumTree::new(&values)
                };
                for step in 0..300 {
                    let x = rng.index(n);
                    let y = rng.index(n);
                    let (l, r) = (x.min(y), x.max(y));
                    let value = rng.value();
                    let context = format!("seed={seed}, n={n}, zeroed={zeroed}, step={step}");
                    match rng.next() % 8 {
                        0 => {
                            seg.modify_point(x, value);
                            values[x] = value;
                        }
                        1 | 2 => {
                            seg.modify_range(l, r, Tag { add: value });
                            for v in &mut values[l..=r] {
                                *v += value;
                            }
                        }
                        3 => {
                            seg.modify_range(0, n - 1, Tag { add: value });
                            for v in &mut values {
                                *v += value;
                            }
                        }
                        4 => assert_eq!(seg.query_point(x), values[x], "{context}"),
                        5 => assert_eq!(seg.range_query(l, r), values[l..=r].iter().sum::<i64>(), "{context}"),
                        6 => assert_eq!(seg.to_vec(), values, "{context}"),
                        _ => {
                            for v in &mut values {
                                *v = rng.value();
                            }
                            seg.build(&values);
                        }
                    }
                    assert_eq!(seg.query_all(), values.iter().sum::<i64>(), "{context}");
                }
                assert_sum_ranges(&mut seg, &values);
            }
        }
    }
}

// 以下类型只用于测试，验证泛型树不依赖求和、加法或标记的字段名。
#[derive(Clone, Copy, Default, Debug, PartialEq)]
struct MaxInfo(i64);

impl SegmentInfo<Tag> for MaxInfo {
    fn merge(left: Self, right: Self) -> Self {
        Self(left.0.max(right.0))
    }
    fn apply(&mut self, t: Tag, _len: usize) {
        self.0 += t.add;
    }
}

#[test]
fn generic_max_queries_handle_negative_values_and_pending_additions() {
    let values = [-8, -3, -10, -6, -12].map(MaxInfo);
    let mut seg = SegTree::<MaxInfo, Tag>::new(&values);
    assert_eq!(seg.query_all(), MaxInfo(-3));
    assert_eq!(seg.range_query(2, 4), MaxInfo(-6));
    seg.modify_range(0, 4, Tag { add: -5 });
    seg.modify_range(1, 3, Tag { add: 2 });
    seg.modify_point(1, MaxInfo(-30));
    assert_eq!(seg.query_all(), MaxInfo(-9));
    assert_eq!(seg.range_query(0, 2), MaxInfo(-13));
    assert_eq!(seg.to_vec(), [-13, -30, -13, -9, -17].map(MaxInfo));

    let mut zeros = SegTree::<MaxInfo, Tag>::with_len(3);
    zeros.modify_point(1, MaxInfo(-10));
    assert_eq!(zeros.query_all(), MaxInfo(0));
    zeros.modify_range(0, 2, Tag { add: -2 });
    assert_eq!(zeros.query_all(), MaxInfo(-2));
}

#[derive(Clone, Copy)]
struct Affine {
    a: i64,
    b: i64,
}

impl Default for Affine {
    fn default() -> Self {
        Self { a: 1, b: 0 }
    }
}

impl LazyTag for Affine {
    fn apply(&mut self, t: Self) {
        self.a *= t.a;
        self.b = t.a * self.b + t.b;
    }
    fn is_empty(&self) -> bool {
        self.a == 1 && self.b == 0
    }
}

#[derive(Clone, Copy, Default, Debug, PartialEq)]
struct AffineSum(i64);

impl SegmentInfo<Affine> for AffineSum {
    fn merge(left: Self, right: Self) -> Self {
        Self(left.0 + right.0)
    }
    fn apply(&mut self, t: Affine, len: usize) {
        self.0 = t.a * self.0 + t.b * len as i64;
    }
}

#[test]
fn generic_affine_tags_apply_in_chronological_order() {
    let values = [1, 2, 3, 4, 5].map(AffineSum);
    let mut seg = SegTree::<AffineSum, Affine>::new(&values);
    seg.modify_range(0, 4, Affine { a: 1, b: 3 });
    seg.modify_range(0, 4, Affine { a: 2, b: 0 });
    assert_eq!(seg.query_all(), AffineSum(60));
    assert_eq!(seg.range_query(1, 3), AffineSum(36));
    seg.modify_range(1, 3, Affine { a: 0, b: -2 });
    seg.modify_range(0, 4, Affine { a: 1, b: 1 });
    seg.modify_point(2, AffineSum(100));
    assert_eq!(seg.to_vec(), [9, -1, 100, -1, 17].map(AffineSum));
    seg.modify_range(0, 4, Affine { a: 0, b: 99 });
    seg.build(&values);
    assert_eq!(seg.to_vec(), values);
}

#[test]
fn seeded_affine_operations_match_an_array() {
    for n in [1, 3, 5, 8, 17, 33] {
        let mut rng = Rng(0xaff1);
        let mut values = vec![0; n];
        let mut seg = SegTree::<AffineSum, Affine>::with_len(n);
        for step in 0..300 {
            let x = rng.index(n);
            let y = rng.index(n);
            let (l, r) = if step % 7 == 0 {
                (0, n - 1)
            } else {
                (x.min(y), x.max(y))
            };
            // a 取 -1/0/1，检查顺序、赋值和负数，同时避免数值无限增长。
            let t = Affine {
                a: (rng.next() % 3) as i64 - 1,
                b: rng.value(),
            };
            if step % 11 == 0 {
                seg.modify_point(x, AffineSum(t.b));
                values[x] = t.b;
            } else {
                seg.modify_range(l, r, t);
                for v in &mut values[l..=r] {
                    *v = t.a * *v + t.b;
                }
            }
            assert_eq!(seg.query_all().0, values.iter().sum::<i64>(), "n={n}, step={step}");
            if step % 5 == 4 {
                assert_eq!(
                    seg.range_query(l, r).0,
                    values[l..=r].iter().sum::<i64>(),
                    "n={n}, step={step}"
                );
                assert_eq!(seg.query_point(x).0, values[x], "n={n}, step={step}");
            }
        }
        assert_eq!(seg.to_vec(), values.into_iter().map(AffineSum).collect::<Vec<_>>());
    }
}

#[derive(Clone, Copy, Default)]
struct Noop;

impl LazyTag for Noop {
    fn apply(&mut self, _t: Self) {}
    fn is_empty(&self) -> bool {
        true
    }
}

#[derive(Clone, Copy, Default, Debug, PartialEq)]
struct Digits {
    value: u64,
    len:   u32,
}

impl SegmentInfo<Noop> for Digits {
    fn merge(left: Self, right: Self) -> Self {
        Self {
            value: left.value * 10u64.pow(right.len) + right.value,
            len:   left.len + right.len,
        }
    }
    fn apply(&mut self, _t: Noop, _len: usize) {}
}

#[test]
fn generic_merge_preserves_left_to_right_order() {
    let values = [1, 2, 3, 4, 5].map(|value| Digits { value, len: 1 });
    let mut seg = SegTree::<Digits, Noop>::new(&values);
    assert_eq!(seg.query_all(), Digits { value: 12345, len: 5 });
    assert_eq!(seg.range_query(1, 3), Digits { value: 234, len: 3 });
    seg.modify_point(2, Digits { value: 9, len: 1 });
    assert_eq!(seg.query_all(), Digits { value: 12945, len: 5 });
    assert_eq!(seg.range_query(1, 3), Digits { value: 294, len: 3 });
}

#[test]
#[should_panic]
fn new_rejects_an_empty_array() {
    let _ = SumTree::new(&[]);
}

#[test]
#[should_panic]
fn with_len_rejects_zero() {
    let _ = SumTree::with_len(0);
}

#[test]
#[should_panic]
fn build_rejects_a_shorter_array() {
    SumTree::with_len(3).build(&[1, 2]);
}

#[test]
#[should_panic]
fn build_rejects_a_longer_array() {
    SumTree::with_len(3).build(&[1, 2, 3, 4]);
}

#[test]
#[should_panic]
fn modify_point_rejects_an_out_of_bounds_index() {
    SumTree::with_len(3).modify_point(3, 10);
}

#[test]
#[should_panic]
fn query_point_rejects_an_out_of_bounds_index() {
    SumTree::with_len(3).query_point(3);
}

#[test]
#[should_panic]
fn modify_range_rejects_reversed_endpoints() {
    SumTree::with_len(3).modify_range(2, 1, Tag { add: 1 });
}

#[test]
#[should_panic]
fn modify_range_rejects_an_out_of_bounds_endpoint() {
    SumTree::with_len(3).modify_range(0, 3, Tag { add: 1 });
}

#[test]
#[should_panic]
fn range_query_rejects_reversed_endpoints() {
    SumTree::with_len(3).range_query(2, 1);
}

#[test]
#[should_panic]
fn range_query_rejects_an_out_of_bounds_endpoint() {
    SumTree::with_len(3).range_query(0, 3);
}
