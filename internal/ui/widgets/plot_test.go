package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestPlotLayout(t *testing.T) {
	plotEmpty := &Plot{}

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

	// Empty data returns empty dimensions
	dimsEmpty := plotEmpty.Layout(gtx, th)
	if dimsEmpty.Size.X != 0 || dimsEmpty.Size.Y != 0 {
		t.Fatalf("expected 0x0 for empty plot, got %+v", dimsEmpty.Size)
	}

	// Populated data series
	plot := &Plot{
		Data: []float32{0.1, 0.5, 0.8, 0.3, 0.9, 0.4},
	}

	dims := plot.Layout(gtx, th)
	if dims.Size.X != 400 || dims.Size.Y != 200 {
		t.Fatalf("expected layout dims 400x200, got %+v", dims.Size)
	}
}
