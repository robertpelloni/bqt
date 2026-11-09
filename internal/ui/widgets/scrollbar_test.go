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

func TestScrollBarHelpersAndZeroTrack(t *testing.T) {
	// 1. Test scrollClamp01
	if scrollClamp01(-0.5) != 0.0 {
		t.Errorf("expected 0.0, got %f", scrollClamp01(-0.5))
	}
	if scrollClamp01(1.5) != 1.0 {
		t.Errorf("expected 1.0, got %f", scrollClamp01(1.5))
	}
	if scrollClamp01(0.7) != 0.7 {
		t.Errorf("expected 0.7, got %f", scrollClamp01(0.7))
	}

	// 2. Test scrollMaxInt
	if scrollMaxInt(15, 10) != 15 {
		t.Errorf("expected 15, got %d", scrollMaxInt(15, 10))
	}
	if scrollMaxInt(4, 9) != 9 {
		t.Errorf("expected 9, got %d", scrollMaxInt(4, 9))
	}
	if scrollMaxInt(7, 7) != 7 {
		t.Errorf("expected 7, got %d", scrollMaxInt(7, 7))
	}

	// 3. Test ScrollDistance getter
	sb := &ScrollBar{Visible: true}
	sb.LastDelta = 0.35
	if sb.ScrollDistance() != 0.35 {
		t.Errorf("expected 0.35, got %f", sb.ScrollDistance())
	}

	// 4. Test zero-dimension track constraints returns empty dimensions
	var ops op.Ops
	gtxZero := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Point{X: 0, Y: 0}),
	}
	th := theme.GetTheme(theme.Dark)
	dimsZero := sb.Layout(gtxZero, th)
	if dimsZero.Size.X != 0 || dimsZero.Size.Y != 0 {
		t.Errorf("expected 0x0 for zero track constraints, got %v", dimsZero.Size)
	}
}
