package widgets

import (
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Plot struct {
	Data []float32
}

func (p *Plot) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if len(p.Data) < 2 { return layout.Dimensions{} }

	// --- GO NATIVE GPU PLOTTER ---
	var path clip.Path
	path.Begin(gtx.Ops)
	
	w := float32(gtx.Constraints.Max.X)
	h := float32(gtx.Constraints.Max.Y)
	step := w / float32(len(p.Data)-1)

	for i, val := range p.Data {
		x := float32(i) * step
		y := h - (val * h)
		if i == 0 {
			path.MoveTo(f32.Pt(x, y))
		} else {
			path.LineTo(f32.Pt(x, y))
		}
	}

	paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
	clip.Stroke{Path: path.End(), Width: 2}.Op().Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	return layout.Dimensions{Size: gtx.Constraints.Max}
}
