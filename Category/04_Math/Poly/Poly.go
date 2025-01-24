const P = 998244353

func nttPow(x, n int) (res int) {
	res = 1
	for ; n > 0; n /= 2 {
		if n%2 > 0 {
			res = res * x % P
		}
		x = x * x % P
	}
	return
}

var omega, omegaInv [31]int

func init() {
	const g, invG = 3, 332748118
	for i := 1; i < len(omega); i++ {
		omega[i] = nttPow(g, (P-1)/(1<<i))
		omegaInv[i] = nttPow(invG, (P-1)/(1<<i))
	}
}

type ntt struct {
	n    int
	invN int
}

func newNTT(n int) ntt { return ntt{n, nttPow(n, P-2)} }

func (t ntt) transform(a, omega []int) {
	for i, j := 0, 0; i < t.n; i++ {
		if i > j {
			a[i], a[j] = a[j], a[i]
		}
		for l := t.n >> 1; ; l >>= 1 {
			j ^= l
			if j >= l {
				break
			}
		}
	}
	for l, li := 2, 1; l <= t.n; l <<= 1 {
		m := l >> 1
		wn := omega[li]
		li++
		for st := 0; st < t.n; st += l {
			b := a[st:]
			for i, w := 0, 1; i < m; i++ {
				d := b[m+i] * w % P
				b[m+i] = (b[i] - d + P) % P
				b[i] = (b[i] + d) % P
				w = w * wn % P
			}
		}
	}
}

func (t ntt) dft(a []int) {
	t.transform(a, omega[:])
}

func (t ntt) idft(a []int) {
	t.transform(a, omegaInv[:])
	for i, v := range a {
		a[i] = v * t.invN % P
	}
}

type poly []int

func (a poly) resize(n int) poly {
	b := make(poly, n)
	copy(b, a)
	return b
}

func (a poly) conv(b poly) poly {
	n, m := len(a), len(b)
	limit := 1 << bits.Len(uint(n+m-1))
	A := a.resize(limit)
	B := b.resize(limit)
	t := newNTT(limit)
	t.dft(A)
	t.dft(B)
	for i, v := range A {
		A[i] = v * B[i] % P
	}
	t.idft(A)
	return A[:n+m-1]
}

func polyConvNTTs(coefs []poly) poly {
	n := len(coefs)
	if n == 1 {
		return coefs[0]
	}
	return polyConvNTTs(coefs[:n/2]).conv(polyConvNTTs(coefs[n/2:]))
}

func (a poly) reverse() poly {
	slices.Reverse(a)
	return a
}

func (a poly) reverseCopy() poly {
	n := len(a)
	b := make(poly, n)
	for i, v := range a {
		b[n-1-i] = v
	}
	return b
}

func (a poly) neg() poly {
	b := make(poly, len(a))
	for i, v := range a {
		if v > 0 {
			b[i] = P - v
		}
	}
	return b
}

func (a poly) add(b poly) poly {
	c := make(poly, len(a))
	for i, v := range a {
		c[i] = (v + b[i]) % P
	}
	return c
}

func (a poly) sub(b poly) poly {
	c := make(poly, len(a))
	for i, v := range a {
		c[i] = (v - b[i] + P) % P
	}
	return c
}

func (a poly) mul(k int) poly {
	k %= P
	b := make(poly, len(a))
	for i, v := range a {
		b[i] = v * k % P
	}
	return b
}

func (a poly) lsh(k int) poly {
	b := make(poly, len(a))
	if k > len(a) {
		return b
	}
	copy(b[k:], a)
	return b
}

func (a poly) rsh(k int) poly {
	b := make(poly, len(a))
	if k > len(a) {
		return b
	}
	copy(b, a[k:])
	return b
}

func (a poly) derivative() poly {
	n := len(a)
	d := make(poly, n)
	for i := 1; i < n; i++ {
		d[i-1] = a[i] * i % P
	}
	return d
}

func (a poly) integral() poly {
	n := len(a)
	s := make(poly, n)
	s[0] = 0

	inv := make([]int, n)
	inv[1] = 1
	for i := 2; i < n; i++ {
		inv[i] = (P - P/i) * inv[P%i] % P
	}
	for i := 1; i < n; i++ {
		s[i] = a[i-1] * inv[i] % P
	}
	return s
}

func (a poly) inv() poly {
	n := len(a)
	m := 1 << bits.Len(uint(n))
	A := a.resize(m)
	invA := make(poly, m)
	invA[0] = nttPow(A[0], P-2)
	for l := 2; l <= m; l <<= 1 {
		ll := l << 1
		b := A[:l].resize(ll)
		iv := invA[:l].resize(ll)
		t := newNTT(ll)
		t.dft(b)
		t.dft(iv)
		for i, v := range iv {
			b[i] = v * (2 - v*b[i]%P + P) % P
		}
		t.idft(b)
		copy(invA, b[:l])
	}
	return invA[:n]
}

func (a poly) div(b poly) poly {
	k := len(a) - len(b) + 1
	if k <= 0 {
		return make(poly, 1)
	}
	A := a.reverseCopy().resize(k)
	B := b.reverseCopy().resize(k)
	return A.conv(B.inv())[:k].reverse()
}

func (a poly) mod(b poly) poly {
	m := len(b)
	return a[:m-1].sub(a.div(b).conv(b)[:m-1])
}

func (a poly) divmod(b poly) (quo, rem poly) {
	m := len(b)
	quo = a.div(b)
	rem = a[:m-1].sub(quo.conv(b)[:m-1])
	return
}

func (a poly) sqrt() poly {
	const inv2 = (P + 1) / 2
	n := len(a)
	m := 1 << bits.Len(uint(n))
	A := a.resize(m)
	rt := make(poly, m)
	rt[0] = 1
	if a[0] != 1 {
		rt[0] = int(new(big.Int).ModSqrt(big.NewInt(int64(a[0])), big.NewInt(P)).Int64())

	}
	for l := 2; l <= m; l <<= 1 {
		ll := l << 1
		b := A[:l].resize(ll)
		r := rt[:l].resize(ll)
		ir := rt[:l].inv().resize(ll)
		t := newNTT(ll)
		t.dft(b)
		t.dft(r)
		t.dft(ir)
		for i, v := range r {
			b[i] = (b[i] + v*v%P) * inv2 % P * ir[i] % P
		}
		t.idft(b)
		copy(rt, b[:l])
	}
	return rt[:n]
}

func (a poly) ln() poly {
	if a[0] != 1 {
		panic(a[0])
	}
	return a.derivative().conv(a.inv())[:len(a)].integral()
}

func (a poly) exp() poly {
	if a[0] != 0 {
		panic(a[0])
	}
	n := len(a)
	m := 1 << bits.Len(uint(n))
	A := a.resize(m)
	e := make(poly, m)
	e[0] = 1
	for l := 2; l <= m; l <<= 1 {
		b := e[:l].ln()
		b[0]--
		for i, v := range b {
			b[i] = (A[i] - v + P) % P
		}
		copy(e, b.conv(e[:l])[:l])
	}
	return e[:n]
}

func (a poly) pow(k int) poly {
	n := len(a)
	if k >= n && a[0] == 0 {
		return make(poly, n)
	}
	k1 := k % (P - 1)
	k %= P
	if a[0] == 1 {
		return a.ln().mul(k).exp()
	}
	shift := 0
	for ; shift < n && a[shift] == 0; shift++ {
	}
	if shift*k >= n {
		return make(poly, n)
	}
	a = a.rsh(shift)
	a.mul(nttPow(a[0], P-2))
	return a.ln().mul(k).exp().mul(nttPow(a[0], k1)).lsh(shift * k)
}

func (a poly) sincos() (sin, cos poly) {
	if a[0] != 0 {
		panic(a[0])
	}
	const i = 911660635
	const inv2i = 43291859
	const inv2 = (P + 1) / 2
	e := a.mul(i).exp()
	invE := e.inv()
	sin = e.sub(invE).mul(inv2i)
	cos = e.add(invE).mul(inv2)
	return
}

func (a poly) tan() poly {
	sin, cos := a.sincos()
	return sin.conv(cos.inv())
}

func (a poly) asin() poly {
	if a[0] != 0 {
		panic(a[0])
	}
	n := len(a)
	b := a.conv(a)[:n].neg()
	b[0] = 1
	return a.derivative().conv(b.sqrt().inv())[:n].integral()
}

func (a poly) acos() poly {
	return a.asin().neg()
}

func (a poly) atan() poly {
	if a[0] != 0 {
		panic(a[0])
	}
	n := len(a)
	b := a.conv(a)[:n]
	b[0] = 1
	return a.derivative().conv(b.inv())[:n].integral()
}

func solve() {
	var n int
	Fscan(in, &n)

	a := make(poly, n)
	for i := range a {
		Fscan(in, &a[i])
	}
	for _, v := range a.ln() {
		Fprint(out, v, " ")
	}
}
