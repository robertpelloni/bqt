package widgets

import (
	"fmt"
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestScrollViewLayout(t *testing.T) {
	sv := NewScrollView(layout.Vertical)
	if sv == nil {
		t.Fatal("expected non-nil ScrollView from NewScrollView")
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 400,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)
	mth := material.NewTheme()

	child := func(gtx layout.Context, i int) layout.Dimensions {
		return material.Label(mth, unit.Sp(12), fmt.Sprintf("Item %d", i)).Layout(gtx)
	}

	// Layout with 50 items to trigger scroll range calculation
	dims := sv.LayoutWithTheme(gtx, th, 50, child)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions for vertical ScrollView, got %+v", dims.Size)
	}

	// Test cross-axis viewport adjustments
	sv.SetCrossAxisViewport(0.2, 0.4)

	// Horizontal ScrollView
	svHoriz := NewScrollView(layout.Horizontal)
	svHoriz.HorizontalPolicy = ScrollAlwaysOn
	dimsHoriz := svHoriz.Layout(gtx, 20, child)
	if dimsHoriz.Size.X <= 0 || dimsHoriz.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions for horizontal ScrollView, got %+v", dimsHoriz.Size)
	}
}

func TestScrollViewOccupiedPlacementAndDeltas(t *testing.T) {
	sv := NewScrollView(layout.Vertical)
	sv.VerticalPolicy = ScrollAlwaysOn
	sv.HorizontalPolicy = ScrollAlwaysOn
	sv.VerticalPlacement = ScrollBarOccupy
	sv.HorizontalPlacement = ScrollBarOccupy
	sv.BarThickness = 16

	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Point{X: 500, Y: 400}),
		Metric:      unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}
	th := theme.GetTheme(theme.Dark)
	mth := material.NewTheme()

	child := func(gtx layout.Context, i int) layout.Dimensions {
		return material.Label(mth, unit.Sp(12), fmt.Sprintf("Occupied Item %d", i)).Layout(gtx)
	}

	dims := sv.LayoutWithTheme(gtx, th, 100, child)
	if dims.Size.X != 500 || dims.Size.Y != 400 {
		t.Errorf("expected dims 500x400, got %v", dims.Size)
	}

	w, h := sv.occupiedSpace()
	if w != 16 || h != 16 {
		t.Errorf("expected occupied space 16x16, got %dx%d", w, h)
	}

	// Verify applyScrollDeltas
	sv.applyScrollDeltas(100)
}
