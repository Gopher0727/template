// https://github.com/EndlessCheng/codeforces-go/blob/master/copypasta/treap/map/map_test.go
func TestInt(tt *testing.T) {
    assert := assert.New(tt)

    t := NewTreap[int, string]()
    assert.Equal(0, t.Size())
    assert.True(t.Empty())
    assert.Nil(t.Min())
    assert.Nil(t.Max())
    assert.Nil(t.Kth(0))
    assert.Nil(t.Kth(1))
    assert.Nil(t.Kth(-1))
    assert.Equal(0, t.Lowerbound(0))
    assert.Equal(0, t.Upperbound(0))
    assert.Nil(t.Prev(0))
    assert.Nil(t.Next(0))
    assert.Nil(t.Find(0))

    t.Put(1, "a")
    assert.Equal(1, t.Size())
    assert.False(t.Empty())
    assert.Equal(1, t.Min().key)
    assert.Equal(1, t.Max().key)
    assert.Equal(1, t.Find(1).key)
    assert.Equal("a", t.Find(1).value)

    t.Put(10, "c")
    t.Put(2, "b")
    t.Put(1, "aa")
    assert.Equal("aa", t.Find(1).value)
    t.Put(2, "bb")
    assert.Equal("bb", t.Find(2).value)
    assert.Equal(1, t.Min().key)
    assert.Equal(10, t.Max().key)
    assert.Nil(t.Find(0))
    assert.NotNil(t.Find(1))
    assert.NotNil(t.Find(2))
    assert.Nil(t.Find(3))

    assert.Equal(1, t.Kth(0).key)
    assert.Equal(2, t.Kth(1).key)
    assert.Equal(10, t.Kth(2).key)
    assert.Nil(t.Kth(-1))
    assert.Nil(t.Kth(3))

    assert.Equal(0, t.Lowerbound(0))
    assert.Equal(0, t.Lowerbound(1))
    assert.Equal(1, t.Lowerbound(2))
    assert.Equal(2, t.Lowerbound(3))
    assert.Equal(2, t.Lowerbound(10))
    assert.Equal(3, t.Lowerbound(11))

    assert.Equal(0, t.Upperbound(0))
    assert.Equal(1, t.Upperbound(1))
    assert.Equal(2, t.Upperbound(2))
    assert.Equal(2, t.Upperbound(9))
    assert.Equal(3, t.Upperbound(10))

    assert.Nil(t.Prev(1))
    assert.Equal(1, t.Prev(2).key)
    assert.Equal(2, t.Prev(9).key)
    assert.Equal(2, t.Prev(10).key)
    assert.Equal(10, t.Prev(11).key)

    assert.Equal(1, t.Next(0).key)
    assert.Equal(2, t.Next(1).key)
    assert.Equal(10, t.Next(2).key)
    assert.Equal(10, t.Next(9).key)
    assert.Nil(t.Next(10))

    t.Delete(1) // 只删除一个 1
    assert.Equal(2, t.Size())

    t.Delete(1) // 无效
    assert.Equal(2, t.Size())

    t.Delete(2)
    assert.Equal(1, t.Size())

    t.Delete(2) // 无效
    assert.Equal(1, t.Size())

    t.Delete(10)
    assert.Equal(0, t.Size())
    assert.True(t.Empty())
    assert.Nil(t.Min())
    assert.Nil(t.Max())
    assert.Nil(t.Kth(0))
    assert.Nil(t.Kth(1))
    assert.Nil(t.Kth(-1))
    assert.Equal(0, t.Lowerbound(0))
    assert.Equal(0, t.Upperbound(0))
    assert.Nil(t.Prev(0))
    assert.Nil(t.Next(0))
}

func TestPair(tt *testing.T) {
    assert := assert.New(tt)

    type pair struct{ x, y int }
    t := NewTreapWith[pair, string](func(a, b pair) int { return cmp.Or(a.x-b.x, a.y-b.y) })
    assert.Equal(0, t.Size())
    assert.True(t.Empty())
    assert.Nil(t.Min())
    assert.Nil(t.Max())

    t.Put(pair{1, 2}, "b")
    assert.Equal(1, t.Size())
    assert.False(t.Empty())
    assert.Equal(pair{1, 2}, t.Min().key)
    assert.Equal(pair{1, 2}, t.Max().key)
    assert.Equal(pair{1, 2}, t.Find(pair{1, 2}).key)
    assert.Equal("b", t.Find(pair{1, 2}).value)

    t.Put(pair{10, 20}, "c")
    t.Put(pair{1, 1}, "a")
    t.Put(pair{1, 2}, "bb")
    assert.Equal("bb", t.Find(pair{1, 2}).value)
    t.Put(pair{1, 1}, "aa")
    assert.Equal("aa", t.Find(pair{1, 1}).value)
    assert.Equal(pair{1, 1}, t.Min().key)
    assert.Equal(pair{10, 20}, t.Max().key)
    assert.NotNil(t.Find(pair{1, 1}))
    assert.Nil(t.Find(pair{1, 0}))
    assert.Nil(t.Find(pair{1, 3}))

    assert.Equal(pair{1, 1}, t.Kth(0).key)
    assert.Equal(pair{1, 2}, t.Kth(1).key)
    assert.Equal(pair{10, 20}, t.Kth(2).key)
    assert.Nil(t.Kth(-1))
    assert.Nil(t.Kth(3))

    assert.Equal(0, t.Lowerbound(pair{}))
    assert.Equal(0, t.Lowerbound(pair{1, 1}))
    assert.Equal(1, t.Lowerbound(pair{1, 2}))
    assert.Equal(2, t.Lowerbound(pair{1, 3}))
    assert.Equal(2, t.Lowerbound(pair{10, 20}))
    assert.Equal(3, t.Lowerbound(pair{10, 21}))

    assert.Equal(pair{1, 1}, t.Prev(pair{1, 2}).key)
    assert.Equal(pair{1, 2}, t.Next(pair{1, 1}).key)

    t.Delete(pair{10, 20})
    t.Delete(pair{1, 2})
    t.Delete(pair{1, 2})
    t.Delete(pair{1, 1})
    t.Delete(pair{1, 1})
    assert.Equal(0, t.Size())
    assert.True(t.Empty())
    assert.Nil(t.Min())
    assert.Nil(t.Max())
}
