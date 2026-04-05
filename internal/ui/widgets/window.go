package widgets

import (
	"image"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Window struct {
	ID    string
	Title string
	Tab   string
	Body  string
	Pos   f32.Point
	Size  f32.Point
}

func (w *Window) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	width := int(w.Size.X)
	height := int(w.Size.Y)
	if width <= 0 {
		width = 220
	}
	if height <= 0 {
		height = 160
	}

	stack := op.Offset(image.Pt(int(w.Pos.X), int(w.Pos.Y))).Push(gtx.Ops)
	defer stack.Pop()

	local := layout.Context{
		Constraints: layout.Exact(image.Pt(width, height)),
		Metric:      gtx.Metric,
		Now:         gtx.Now,
		Locale:      gtx.Locale,
		Values:      gtx.Values,
		Source:      gtx.Source,
		Ops:         gtx.Ops,
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	outer := image.Rect(0, 0, width, height)
	titleBar := image.Rect(0, 0, width, 30)
	bodyRect := image.Rect(0, 30, width, height)
	paint.FillShape(local.Ops, th.Surface, clip.Rect(outer).Op())
	paint.FillShape(local.Ops, th.Primary, clip.Rect(titleBar).Op())
	paint.FillShape(local.Ops, th.Background, clip.Rect(bodyRect).Op())
	if th.Type == theme.Cyberpunk {
		for i := 40; i < height; i += 40 {
			line := image.Rect(0, i, width, i+1)
			paint.FillShape(local.Ops, th.Primary, clip.Rect(line).Op())
		}
	}

	return layout.Inset{Top: unit.Dp(8), Left: unit.Dp(10), Right: unit.Dp(10), Bottom: unit.Dp(8)}.Layout(local, func(gtx layout.Context) layout.Dimensions {
		body := w.Body
		if body == "" {
			body = "Managed window baseline"
		}
		tabInfo := w.Tab
		if tabInfo == "" {
			tabInfo = "shared"
		}
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(material.Label(mth, unit.Sp(12), w.Title).Layout),
			layout.Rigid(layout.Spacer{Height: unit.Dp(18)}.Layout),
			layout.Rigid(material.Body2(mth, body).Layout),
			layout.Rigid(layout.Spacer{Height: unit.Dp(8)}.Layout),
			layout.Rigid(material.Caption(mth, "tab="+tabInfo+" id="+w.ID).Layout),
		)
	})
}
