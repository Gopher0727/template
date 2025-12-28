type Vec []int

type Matrix struct {
    r   int
    c   int
    mat Vec
}

func NewMatrix(r, c int) *Matrix {
    if r < 0 || c < 0 {
        panic("r and c must be non-negative")
    }
    return &Matrix{
        r:   r,
        c:   c,
        mat: make(Vec, r*c),
    }
}

func (m *Matrix) At(i, j int) int {
    if i < 0 || i >= m.r || j < 0 || j >= m.c {
        panic("At(): index out of range")
    }
    return m.mat[i*m.c+j]
}

func (m *Matrix) Set(i, j, val int) {
    if i < 0 || i >= m.r || j < 0 || j >= m.c {
        panic("Set(): index out of range")
    }
    m.mat[i*m.c+j] = val
}

func (m *Matrix) String() string {
    if m.r == 0 || m.c == 0 {
        return "[]\n"
    }

    mx := 0
    for _, v := range m.mat {
        mx = max(mx, len(fmt.Sprintf("%d", v)))
    }

    var s strings.Builder
    for i := range m.r {
        s.WriteString("[")
        for j := range m.c {
            fmt.Fprintf(&s, "%*d", mx, m.At(i, j))
            if j != m.c-1 {
                s.WriteString(" ")
            }
        }
        s.WriteString("]\n")
    }
    return s.String()
}

func Identity(r int) *Matrix {
    m := NewMatrix(r, r)
    for i := range r {
        m.mat[i*m.c+i] = 1
    }
    return m
}

func (m *Matrix) IsSquare() bool { return m.r == m.c }

func (m *Matrix) Transpose() *Matrix {
    res := NewMatrix(m.c, m.r)
    for i := range m.r {
        for j := range m.c {
            res.mat[j*res.c+i] = m.mat[i*m.c+j]
        }
    }
    return res
}

func (m *Matrix) Form() []Vec {
    res := make([]Vec, m.r)
    for i := range m.r {
        res[i] = make(Vec, m.c)
        for j := range m.c {
            res[i][j] = m.At(i, j)
        }
    }
    return res
}

func (m *Matrix) Mul(o *Matrix) *Matrix {
    if m.c != o.r {
        panic("Mul(): incompatible matrix sizes")
    }
    res := NewMatrix(m.r, o.c)
    for i := range m.r {
        for k := range m.c {
            if m.At(i, k) == 0 {
                continue
            }
            for j := range o.c {
                res.mat[i*res.c+j] += m.At(i, k) * o.At(k, j)
            }
        }
    }
    return res
}

func (m *Matrix) MulVec(v Vec) Vec {
    if m.c != len(v) {
        panic("MulVec(): incompatible matrix and vector sizes")
    }
    res := make(Vec, m.r)
    for i := range m.r {
        for j := range m.c {
            res[i] += m.At(i, j) * v[j]
        }
    }
    return res
}

func (m *Matrix) Pow(n int) *Matrix {
    if n < 0 {
        panic("Pow(): n must be non-negative")
    }
    if !m.IsSquare() {
        panic("Pow(): matrix must be square")
    }
    res := Identity(m.r)
    a := m
    for n > 0 {
        if n&1 == 1 {
            res = res.Mul(a)
        }
        a = a.Mul(a)
        n >>= 1
    }
    return res
}
