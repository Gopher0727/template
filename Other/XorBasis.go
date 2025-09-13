type Integer interface {
    ~int | ~int32 | ~int64
}

type XorBasis[T Integer] struct {
    basis   []T
    cnt     int
    canZero bool
}

func NewXorBasis[T Integer]() *XorBasis[T] {
    return &XorBasis[T]{basis: make([]T, int(unsafe.Sizeof(*new(T)))*8)}
}

func (xor *XorBasis[T]) Insert(x T) bool {
    for i := len(xor.basis) - 1; i >= 0; i-- {
        if x>>i&1 == 0 {
            continue
        }
        if xor.basis[i] == 0 {
            xor.basis[i] = x
            xor.cnt++
            return true
        }
        x ^= xor.basis[i]
    }
    xor.canZero = true
    return false
}

func (xor *XorBasis[T]) MaxXor() (ans T) {
    for i := len(xor.basis) - 1; i >= 0; i-- {
        ans = max(ans, ans^xor.basis[i])
    }
    return
}

func (xor *XorBasis[T]) MinXor() (ans T) {
    if !xor.canZero {
        for i := range len(xor.basis) {
            if xor.basis[i] > 0 {
                return xor.basis[i]
            }
        }
    }
    return
}

func (xor *XorBasis[T]) Merge(other XorBasis[T]) {
    for i := len(other.basis) - 1; i >= 0; i-- {
        if other.basis[i] > 0 {
            xor.Insert(other.basis[i])
        }
    }
}

func (xor *XorBasis[T]) Decompose(x T) bool {
    for i := len(xor.basis) - 1; i >= 0; i-- {
        if x>>i&1 == 1 {
            if xor.basis[i] == 0 {
                return false
            }
            x ^= xor.basis[i]
        }
    }
    return true
}
