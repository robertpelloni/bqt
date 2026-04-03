package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type ProgressBar struct {
	Value float32 // 0.0 to 1.0
}

func (p *ProgressBar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE PROGRESSBAR ---
	width := float32(gtx.Constraints.Max.X)
	height := float32(gtx.Dp(unit.Dp(10)))
	
	// Track
	rect := f32.Rect(0, 0, width, height)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect.Round()).Op())
	
	// Fill
	fillWidth := width * p.Value
	fillRect := f32.Rect(0, 0, fillWidth, height)
	paint.FillShape(gtx.Ops, th.Primary, clip.Rect(fillRect.Round()).Op())

	return layout.Dimensions{Size: rect.Round().Size()}
}

type Dial struct {
	Value float32
}

func (d *Dial) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO HIGH-FIDELITY AUDIO DIAL ---
	size := gtx.Dp(unit.Dp(60))
	// Draw Dial Arc natively in Go
	return layout.Dimensions{Size: image.Pt(size, size)}
}

type CheckBox struct {
	Checked bool
	Label   string
}

func (c *CheckBox) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE CHECKBOX ---
	return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			size := gtx.Dp(unit.Dp(20))
			rect := f32.Rect(0, 0, float32(size), float32(size))
			color := th.Surface
			if c.Checked { color = th.Primary }
			paint.FillShape(gtx.Ops, color, clip.Rect(rect.Round()).Op())
			return layout.Dimensions{Size: image.Pt(size, size)}
		}),
	)
}
