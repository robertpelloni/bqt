package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestAuthShellLayoutAndBoot(t *testing.T) {
	auth := &AuthShell{}

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

	// Layout verification
	dims := auth.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions for AuthShell, got %+v", dims.Size)
	}

	// Click Boot button and re-layout to exercise database verification query branch
	auth.BootBtn.Click()
	dimsClicked := auth.Layout(gtx, th)
	if dimsClicked.Size.X <= 0 || dimsClicked.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions after boot button click, got %+v", dimsClicked.Size)
	}
}
