package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestShaderEffect_PlasmaLayout(t *testing.T) {
	effect := &ShaderEffect{
		Type: "plasma",
		Time: 1.5,
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 640, Y: 480}),
	}
	th := theme.GetTheme(theme.Dark)

	dims := effect.Layout(gtx, th)
	if dims.Size.X != 640 || dims.Size.Y != 480 {
		t.Errorf("expected dims 640x480, got %v", dims.Size)
	}
}

func TestShaderEffect_FallbackGridPatternLayout(t *testing.T) {
	effect := &ShaderEffect{
		Type: "grid",
		Time: 0.0,
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 320, Y: 240}),
	}
	th := theme.GetTheme(theme.Cyberpunk)

	dims := effect.Layout(gtx, th)
	if dims.Size.X != 320 || dims.Size.Y != 240 {
		t.Errorf("expected dims 320x240, got %v", dims.Size)
	}
}
