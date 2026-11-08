package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestHeaderBar(t *testing.T) {
	hb := &BQtHeaderBar{
		Title:    "Test Title",
		Subtitle: "Test Subtitle",
	}

	gtx := layout.Context{
		Ops:         new(op.Ops),
		Constraints: layout.Exact(image.Point{X: 800, Y: 60}),
	}
	th := theme.GetTheme(theme.Dark)

	dims := hb.Layout(gtx, th)
	if dims.Size.X == 0 && dims.Size.Y == 0 {
		t.Logf("HeaderBar successfully initialized and laid out with dummy dimensions.")
	}

	// Test with populated custom widgets
	widget1Called := false
	widget2Called := false
	hb.CustomWidgets = []layout.Widget{
		func(gtx layout.Context) layout.Dimensions {
			widget1Called = true
			return layout.Dimensions{Size: image.Point{X: 30, Y: 30}}
		},
		func(gtx layout.Context) layout.Dimensions {
			widget2Called = true
			return layout.Dimensions{Size: image.Point{X: 30, Y: 30}}
		},
	}

	dimsWithWidgets := hb.Layout(gtx, th)
	if !widget1Called || !widget2Called {
		t.Errorf("expected all custom widgets to be laid out, got w1=%v, w2=%v", widget1Called, widget2Called)
	}
	if dimsWithWidgets.Size.X <= 0 {
		t.Errorf("expected non-zero width with custom widgets, got %d", dimsWithWidgets.Size.X)
	}
}
