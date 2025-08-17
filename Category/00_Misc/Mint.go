type Mint struct {
    v int
}

func newMint(u int) Mint {
    v := u % MOD
    if v < 0 {
        v += MOD
    }
    return Mint{v}
}

func (m Mint) toString() string {
    return fmt.Sprintf("%d", m.v)
}

func (m Mint) Val() int {
    return m.v
}

// // immutable
func (m Mint) Add(a Mint) Mint {
    r := m.v + a.v
    if r >= MOD {
        r -= MOD
    }
    return Mint{r}
}

func (m Mint) Sub(a Mint) Mint {
    r := m.v - a.v
    if r < 0 {
        r += MOD
    }
    return Mint{r}
}

func (m Mint) mul(a Mint) Mint {
    return Mint{m.v * a.v % MOD}
}

func (m Mint) Neg() Mint {
    if m.v == 0 {
        return Mint{0}
    }
    return Mint{MOD - m.v}
}

func (m Mint) qpow(exp int) (res Mint) {
    res = Mint{1}
    base := m
    for exp > 0 {
        if exp&1 == 1 {
            res = res.mul(base)
        }
        base = base.mul(base)
        exp >>= 1
    }
    return
}

func (m Mint) Inv() (Mint, error) {
    iv, ok := invHelp(m.v, MOD)
    if !ok {
        return Mint{}, errors.New("inverse does not exist")
    }
    return Mint{iv}, nil
}

func (m Mint) Div(a Mint) (Mint, error) {
    ia, err := a.Inv()
    if err != nil {
        return Mint{}, err
    }
    return m.mul(ia), nil
}

func invHelp(a, m int) (int, bool) {
    if m <= 0 {
        return 0, false
    }
    a = ((a % m) + m) % m
    if a == 0 {
        return 0, false
    }
    s, t := m, a
    x0, x1 := 0, 1
    for t != 0 {
        q := s / t
        s, t = t, s-q*t
        x0, x1 = x1, x0-q*x1
    }
    if s != 1 {
        return 0, false
    }
    if x0 < 0 {
        x0 += m
    }
    return x0, true
}
