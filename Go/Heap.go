type T []int

type Heap struct {
	w *wrapper
}

func NewHeap(cmp func(a, b T) bool) *Heap { return &Heap{w: &wrapper{cmp: cmp}} }

func (h *Heap) Push(x T) { heap.Push(h.w, x) }

func (h *Heap) Pop() (_ T) {
	if h.w.Len() == 0 {
		return
	}
	return heap.Pop(h.w).(T)
}

func (h *Heap) Len() int { return h.w.Len() }

func (h *Heap) String() string {
	tmp := &wrapper{
		a:   append([]T(nil), h.w.a...),
		cmp: h.w.cmp,
	}
	heap.Init(tmp)

	res := make([]T, 0, tmp.Len())
	for tmp.Len() > 0 {
		res = append(res, heap.Pop(tmp).(T))
	}
	return fmt.Sprint(res)
}

// 内部 heap wrapper，负责实现 heap.Interface
type wrapper struct {
	a   []T
	cmp func(a, b T) bool
}

func (w *wrapper) Len() int { return len(w.a) }

func (w *wrapper) Less(i, j int) bool { return w.cmp(w.a[i], w.a[j]) }

func (w *wrapper) Swap(i, j int) { w.a[i], w.a[j] = w.a[j], w.a[i] }

func (w *wrapper) Push(x any) { w.a = append(w.a, x.(T)) }

func (w *wrapper) Pop() any { n := len(w.a); x := w.a[n-1]; w.a = w.a[:n-1]; return x }
