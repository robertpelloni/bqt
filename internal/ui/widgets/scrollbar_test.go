package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestScrollBarViewportAndLayout(t *testing.T) {
	sb := &ScrollBar{
		Orientation: ScrollVertical,
		Visible:     true,
	}

	sb.SetViewport(1.5, -0.5) // Should clamp position to 1.0, pageSize to 0.2
	if sb.Position != 1.0 {
		t.Fatalf("expected clamped position 1.0, got %f", sb.Position)
	}
	if sb.PageSize != 0.2 {
		t.Fatalf("expected fallback pageSize 0.2, got %f", sb.PageSize)
	}

	sb.SetViewport(0.3, 0.4)
	if sb.Position != 0.3 || sb.PageSize != 0.4 {
		t.Fatalf("expected 0.3 and 0.4, got %f and %f", sb.Position, sb.PageSize)
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 12,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)
	dims := sb.Layout(gtx, th)

	if dims.Size.X != 12 || dims.Size.Y != 300 {
		t.Fatalf("expected dimensions 12x300, got %+v", dims.Size)
	}

	// Test horizontal layout
	sbHoriz := &ScrollBar{
		Orientation: ScrollHorizontal,
		Visible:     true,
	}
	sbHoriz.SetViewport(0.2, 0.5)

	gtxHoriz := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 300,
			Y: 12,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}
	dimsHoriz := sbHoriz.Layout(gtxHoriz, th)
	if dimsHoriz.Size.X != 300 || dimsHoriz.Size.Y != 12 {
		t.Fatalf("expected horizontal dimensions 300x12, got %+v", dimsHoriz.Size)
	}

	// Test hidden scrollbar returns empty dimensions
	sb.Visible = false
	dimsHidden := sb.Layout(gtx, th)
	if dimsHidden.Size.X != 0 || dimsHidden.Size.Y != 0 {
		t.Fatalf("expected 0x0 for hidden scrollbar, got %+v", dimsHidden.Size)
	}
}
