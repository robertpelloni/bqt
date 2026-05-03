package widgets

import (
	"image"
	"image/color"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
)

// DrawGrabGlow renders a simple multi-pass halo around a rectangle.
func DrawGrabGlow(gtx layout.Context, rect image.Rectangle, accent color.NRGBA) {
	for i := 1; i <= 4; i++ {
		grow := i * 2
		r := image.Rect(rect.Min.X-grow, rect.Min.Y-grow, rect.Max.X+grow, rect.Max.Y+grow)
		c := accent
		c.A = uint8(20 / i)
		paint.FillShape(gtx.Ops, c, clip.Rect(r).Op())
	}
}
