package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestToolTipLayout(t *testing.T) {
	tt := &ToolTip{
		Visible: false,
		Text:    "Tooltip information text",
		X:       50,
		Y:       100,
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

	// When hidden, should return empty dimensions
	dims := tt.Layout(gtx, th)
	if dims.Size.X != 0 || dims.Size.Y != 0 {
		t.Fatalf("expected 0x0 for hidden tooltip, got %+v", dims.Size)
	}

	// When visible but empty text, should return empty dimensions
	tt.Visible = true
	tt.Text = ""
	dimsEmpty := tt.Layout(gtx, th)
	if dimsEmpty.Size.X != 0 || dimsEmpty.Size.Y != 0 {
		t.Fatalf("expected 0x0 for empty text tooltip, got %+v", dimsEmpty.Size)
	}

	// When visible and populated text, should render non-zero dimensions
	tt.Text = "Tooltip information text"
	dimsRendered := tt.Layout(gtx, th)
	if dimsRendered.Size.X <= 0 || dimsRendered.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions for visible tooltip, got %+v", dimsRendered.Size)
	}
}
