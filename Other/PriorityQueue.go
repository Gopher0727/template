type T [4]int64
pq, _ := NewPriorityQueue[T](func(a, b T) bool {

	return false
})

// template
type PriorityQueue[T any] struct {
	heaps    []T
	lessFunc func(a, b T) bool
}

func NewPriorityQueue[T any](less func(a, b T) bool) (*PriorityQueue[T], error) {
	if less == nil {
		return nil, errors.New("less func is necessary")
	}
	return &PriorityQueue[T]{lessFunc: less}, nil
}

func (pq *PriorityQueue[T]) Push(t T) {
	pq.heaps = append(pq.heaps, t)
	pq.up(len(pq.heaps) - 1)
}

func (pq *PriorityQueue[T]) Top() T {
	return pq.heaps[0]
}

func (pq *PriorityQueue[T]) Pop() {
	if len(pq.heaps) <= 1 {
		pq.heaps = nil
		return
	}
	pq.swap(0, len(pq.heaps)-1)
	pq.heaps = pq.heaps[:len(pq.heaps)-1]
	pq.down(0)
}

func (pq *PriorityQueue[T]) Empty() bool {
	return len(pq.heaps) == 0
}

func (pq *PriorityQueue[T]) Size() int {
	return len(pq.heaps)
}

func (pq *PriorityQueue[T]) swap(i, j int) {
	pq.heaps[i], pq.heaps[j] = pq.heaps[j], pq.heaps[i]
}

func (pq *PriorityQueue[T]) up(child int) {
	if child <= 0 {
		return
	}
	parent := (child - 1) >> 1
	if !pq.lessFunc(pq.heaps[child], pq.heaps[parent]) {
		return
	}
	pq.swap(child, parent)
	pq.up(parent)
}

func (pq *PriorityQueue[T]) down(parent int) {
	lessIdx := parent
	lChild, rChild := (parent<<1)+1, (parent<<1)+2
	if lChild < len(pq.heaps) && pq.lessFunc(pq.heaps[lChild], pq.heaps[lessIdx]) {
		lessIdx = lChild
	}
	if rChild < len(pq.heaps) && pq.lessFunc(pq.heaps[rChild], pq.heaps[lessIdx]) {
		lessIdx = rChild
	}
	if lessIdx == parent {
		return
	}
	pq.swap(lessIdx, parent)
	pq.down(lessIdx)
}
