package widgets

import (
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"image/color"
)

type VectorIcon struct {
	Paths []f32.Point
	Color color.NRGBA
}

func (v *VectorIcon) Layout(gtx layout.Context) layout.Dimensions {
	// --- GO VECTOR RENDER PASS ---
	// High-fidelity geometric path construction natively on the GPU.
	var path clip.Path
	path.Begin(gtx.Ops)
	
	if len(v.Paths) > 0 {
		path.MoveTo(v.Paths[0])
		for i := 1; i < len(v.Paths); i++ {
			path.LineTo(v.Paths[i])
		}
		path.Close()
	}

	stack := clip.Outline{Path: path.End()}.Op().Push(gtx.Ops)
	paint.Fill(gtx.Ops, v.Color)
	stack.Pop()

	return layout.Dimensions{Size: gtx.Constraints.Max}
}
