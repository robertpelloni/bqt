package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestLoginViewLayoutAndAuthentication(t *testing.T) {
	login := &LoginView{}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 500,
			Y: 400,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	// Layout when unauthenticated
	dims := login.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions when unauthenticated, got %+v", dims.Size)
	}

	// Trigger Login button click
	login.LoginBtn.Click()
	dimsClicked := login.Layout(gtx, th)

	if !login.Authenticated {
		t.Fatal("expected Authenticated to be true after clicking LoginBtn")
	}

	// When authenticated, Layout returns empty dimensions
	dimsAuth := login.Layout(gtx, th)
	if dimsAuth.Size.X != 0 || dimsAuth.Size.Y != 0 {
		t.Fatalf("expected 0x0 layout dimensions after authentication, got %+v", dimsAuth.Size)
	}
	_ = dimsClicked
}
