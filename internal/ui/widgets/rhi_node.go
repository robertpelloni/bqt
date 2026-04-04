package widgets

import (
	"image"
	"image/color"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
)

type RHINode struct {
	NativeTextureHandle uintptr
}

func (r *RHINode) Layout(gtx layout.Context) layout.Dimensions {
	if r.NativeTextureHandle == 0 {
		return layout.Dimensions{}
	}
	rect := image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)
	paint.FillShape(gtx.Ops, color.NRGBA{R: 80, G: 120, B: 200, A: 255}, clip.Rect(rect).Op())
	return layout.Dimensions{Size: rect.Size()}
}
