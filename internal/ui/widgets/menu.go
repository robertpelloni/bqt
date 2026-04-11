package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type MenuItem struct {
	Label   string
	Action  string
	Clicked widget.Clickable
}

// Menu is a lightweight Go-native QuickControls2-style popup menu.
type Menu struct {
	Visible bool
	Items   []MenuItem
}

func (m *Menu) Layout(gtx layout.Context, th theme.Theme) (layout.Dimensions, string) {
	if !m.Visible {
		return layout.Dimensions{}, ""
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	chosen := ""
	list := layout.List{Axis: layout.Vertical}
	dims := list.Layout(gtx, len(m.Items), func(gtx layout.Context, i int) layout.Dimensions {
		if m.Items[i].Clicked.Clicked(gtx) {
			chosen = m.Items[i].Action
		}
		btn := material.Button(mth, &m.Items[i].Clicked, m.Items[i].Label)
		return layout.Inset{Top: unit.Dp(2), Bottom: unit.Dp(2)}.Layout(gtx, btn.Layout)
	})
	return dims, chosen
}
