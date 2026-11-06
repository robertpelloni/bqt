package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
)

func TestGrid_Layout(t *testing.T) {
	grid := &Grid{
		Rows: 2,
		Cols: 2,
		Gap:  unit.Dp(8),
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Pt(200, 200)),
	}

	// 1. Layout with empty children should not panic
	dims := grid.Layout(gtx, nil)
	if dims.Size.X == 0 && dims.Size.Y == 0 {
		t.Log("Grid layout handled empty children correctly")
	}

	// 2. Layout with populated child
	childCalled := false
	child := func(gtx layout.Context) layout.Dimensions {
		childCalled = true
		return layout.Dimensions{Size: image.Pt(80, 80)}
	}

	dims = grid.Layout(gtx, []layout.Widget{child})
	if !childCalled {
		t.Errorf("Expected child widget to be laid out")
	}
	if dims.Size.X == 0 || dims.Size.Y == 0 {
		t.Errorf("Expected non-zero dimensions from Grid.Layout, got %v", dims)
	}
}
