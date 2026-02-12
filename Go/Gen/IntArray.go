package main

import (
    "fmt"

    "pgregory.net/rapid"
)

type IntArray struct {
    minV int
    maxV int
    minL int
    maxL int
}

type Option func(*IntArray)

func MinValue(v int) Option  { return func(a *IntArray) { a.minV = v } }
func MaxValue(v int) Option  { return func(a *IntArray) { a.maxV = v } }
func Length(n int) Option    { return func(a *IntArray) { a.minL = n; a.maxL = n } }
func MinLength(n int) Option { return func(a *IntArray) { a.minL = n } }
func MaxLength(n int) Option { return func(a *IntArray) { a.maxL = n } }

func NewIntArray(options ...func(*IntArray)) []int {
    a := &IntArray{
        minV: -1e9,
        maxV: 1e9,
        minL: 0,
        maxL: 30,
    }
    for _, op := range options {
        op(a)
    }
    if a.minL > a.maxL || a.minV > a.maxV {
        panic("invalid options")
    }
    return rapid.SliceOfN(rapid.IntRange(a.minV, a.maxV), a.minL, a.maxL).Example()
}

func main() {
    a := NewIntArray(MinValue(-20), MaxValue(20), MinLength(5), MaxLength(10))
    fmt.Println(a)
}
