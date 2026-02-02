type Point struct {
    x, y int
}

func (p Point) Add(q Point) Point { return Point{p.x + q.x, p.y + q.y} }
func (p Point) Sub(q Point) Point { return Point{p.x - q.x, p.y - q.y} }
func (p Point) Dot(q Point) int   { return p.x*q.x + p.y*q.y }
func (p Point) Cross(q Point) int { return p.x*q.y + p.y*q.x }
func Dot(p, q Point) int          { return p.x*q.x + p.y*q.y }
func Cross(p, q Point) int        { return p.x*q.y - p.y*q.x }

func (p *Point) AddAssign(q Point) {
    p.x += q.x
    p.y += q.y
}

func (p *Point) SubAssign(q Point) {
    p.x -= q.x
    p.y -= q.y
}

func ConvexHull(points []Point) (hi []Point, lo []Point) {
    slices.SortFunc(points, func(a, b Point) int {
        if a.x == b.x {
            return a.y - b.y
        }
        return a.x - b.x
    })

    for _, p := range points {
        for len(hi) > 1 && Cross(hi[len(hi)-1].Sub(hi[len(hi)-2]), p.Sub(hi[len(hi)-1])) >= 0 {
            hi = hi[:len(hi)-1]
        }
        for len(hi) > 0 && hi[len(hi)-1].x == p.x {
            hi = hi[:len(hi)-1]
        }
        hi = append(hi, p)

        for len(lo) > 1 && Cross(lo[len(lo)-1].Sub(lo[len(lo)-2]), p.Sub(lo[len(lo)-1])) <= 0 {
            lo = lo[:len(lo)-1]
        }
        if len(lo) == 0 || lo[len(lo)-1].x < p.x {
            lo = append(lo, p)
        }
    }
    return
}
