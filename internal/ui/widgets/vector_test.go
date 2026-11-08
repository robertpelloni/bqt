package widgets

import (
	"image"
	"image/color"
	"testing"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
)

func TestVectorIconLayout(t *testing.T) {
	vecEmpty := &VectorIcon{
		Color: color.NRGBA{R: 255, G: 0, B: 0, A: 255},
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 100,
			Y: 100,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	dimsEmpty := vecEmpty.Layout(gtx)
	if dimsEmpty.Size.X != 100 || dimsEmpty.Size.Y != 100 {
		t.Fatalf("expected layout dims (100, 100), got %+v", dimsEmpty.Size)
	}

	// Populated geometric triangle path
	vec := &VectorIcon{
		Paths: []f32.Point{
			f32.Pt(10, 10),
			f32.Pt(90, 10),
			f32.Pt(50, 90),
		},
		Color: color.NRGBA{R: 0, G: 255, B: 100, A: 255},
	}

	dims := vec.Layout(gtx)
	if dims.Size.X != 100 || dims.Size.Y != 100 {
		t.Fatalf("expected layout dims (100, 100), got %+v", dims.Size)
	}
}
