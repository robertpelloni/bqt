package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestRewindSliderLayout(t *testing.T) {
	rewind := &RewindSlider{
		Active: false,
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

	th := theme.GetTheme(theme.Dark)

	// Inactive returns empty dimensions
	dimsEmpty := rewind.Layout(gtx, th)
	if dimsEmpty.Size.X != 0 || dimsEmpty.Size.Y != 0 {
		t.Fatalf("expected 0x0 for inactive RewindSlider, got %+v", dimsEmpty.Size)
	}

	// Active layout
	rewind.Active = true
	rewind.Slider.Value = 0.5
	dims := rewind.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions for active RewindSlider, got %+v", dims.Size)
	}
}
