package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestDrawGrabGlow(t *testing.T) {
	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 300,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Cyberpunk)
	rect := image.Rect(50, 50, 150, 150)

	// DrawGrabGlow should write paint ops without panics
	DrawGrabGlow(gtx, rect, th.Accent)
}
