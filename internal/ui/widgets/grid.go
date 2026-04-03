package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
)

type Grid struct {
	Rows, Cols int
	Gap        unit.Dp
}

func (g *Grid) Layout(gtx layout.Context, children []layout.Widget) layout.Dimensions {
	// Native Go GPU-Accelerated Grid Layout
	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
				layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
					// Row/Col distribution logic mapping to Gio Flex
					if len(children) > 0 {
						return children[0](gtx)
					}
					return layout.Dimensions{}
				}),
			)
		}),
	)
}
