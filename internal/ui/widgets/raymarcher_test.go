package widgets

import (
	"image"
	"testing"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
)

func TestRaymarcher_Layout_WithSize(t *testing.T) {
	rm := &Raymarcher{
		Time: 1.0,
		Size: f32.Point{X: 100, Y: 100},
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 400, Y: 300}),
	}

	dims := rm.Layout(gtx)
	if dims.Size.X != 400 || dims.Size.Y != 300 {
		t.Errorf("expected dims 400x300, got %v", dims.Size)
	}
}

func TestRaymarcher_Layout_ZeroSizeFallback(t *testing.T) {
	rm := &Raymarcher{
		Time: 0.0,
		Size: f32.Point{X: 0, Y: 0},
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 200, Y: 200}),
	}

	dims := rm.Layout(gtx)
	if dims.Size.X != 200 || dims.Size.Y != 200 {
		t.Errorf("expected dims 200x200, got %v", dims.Size)
	}
}

func TestRaymarcher_maxInt(t *testing.T) {
	if maxInt(10, 5) != 10 {
		t.Errorf("expected 10, got %d", maxInt(10, 5))
	}
	if maxInt(3, 8) != 8 {
		t.Errorf("expected 8, got %d", maxInt(3, 8))
	}
	if maxInt(4, 4) != 4 {
		t.Errorf("expected 4, got %d", maxInt(4, 4))
	}
}
