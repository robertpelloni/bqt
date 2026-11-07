package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
)

func TestSplitViewLayout(t *testing.T) {
	sv := &SplitView{
		Ratio: 0.0, // Should fallback to 0.5
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 400,
			Y: 200,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	firstCalled := false
	secondCalled := false

	firstWidget := func(gtx layout.Context) layout.Dimensions {
		firstCalled = true
		return layout.Dimensions{Size: gtx.Constraints.Min}
	}
	secondWidget := func(gtx layout.Context) layout.Dimensions {
		secondCalled = true
		return layout.Dimensions{Size: gtx.Constraints.Min}
	}

	dims := sv.Layout(gtx, firstWidget, secondWidget)

	if !firstCalled || !secondCalled {
		t.Fatalf("expected both split children to be laid out, first=%v, second=%v", firstCalled, secondCalled)
	}

	if sv.Ratio != 0.5 {
		t.Fatalf("expected ratio to fallback to 0.5, got %f", sv.Ratio)
	}

	if dims.Size.X != 400 || dims.Size.Y != 200 {
		t.Fatalf("expected dims 400x200, got %+v", dims.Size)
	}
}
