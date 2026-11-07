package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestDrawerLayout(t *testing.T) {
	d := &Drawer{
		Visible: false,
		Title:   "Navigation Drawer",
		WidthDp: 0, // Fallback to 280
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 600,
			Y: 400,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	childRendered := false
	child := func(gtx layout.Context) layout.Dimensions {
		childRendered = true
		return layout.Dimensions{Size: gtx.Constraints.Min}
	}

	// Hidden drawer returns empty dimensions
	dimsHidden := d.Layout(gtx, th, child)
	if dimsHidden.Size.X != 0 || dimsHidden.Size.Y != 0 {
		t.Fatalf("expected 0x0 for hidden drawer, got %+v", dimsHidden.Size)
	}
	if childRendered {
		t.Fatal("expected child not to be rendered when drawer is hidden")
	}

	// Visible drawer renders child and falls back to 280dp
	d.Visible = true
	dims := d.Layout(gtx, th, child)
	if !childRendered {
		t.Fatal("expected child to be rendered when drawer is visible")
	}
	if d.WidthDp != 280 {
		t.Fatalf("expected fallback width 280, got %f", d.WidthDp)
	}
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions for visible drawer, got %+v", dims.Size)
	}

	// Custom width drawer
	d.WidthDp = 320
	dimsCustom := d.Layout(gtx, th, child)
	if dimsCustom.Size.X <= 0 || dimsCustom.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions for custom width drawer, got %+v", dimsCustom.Size)
	}
}
