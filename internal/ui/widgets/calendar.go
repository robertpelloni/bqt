package widgets

import (
	"fmt"
	"time"
	"gioui.org/layout"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Calendar struct {
	CurrentDate time.Time
}

func (c *Calendar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if c.CurrentDate.IsZero() { c.CurrentDate = time.Now() }

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	now := c.CurrentDate
	first := time.Date(now.Year(), now.Month(), 1, 0, 0, 0, 0, time.Local)
	startDay := int(first.Weekday())
	daysInMonth := time.Date(now.Year(), now.Month()+1, 0, 0, 0, 0, 0, time.Local).Day()

	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.H6(mth, fmt.Sprintf("%s %d", now.Month(), now.Year())).Layout(gtx)
		}),
		layout.Rigid(layout.Spacer{Height: unit.Dp(10)}.Layout),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			// --- GO CALENDAR MATRIX RENDER ---
			return layout.Grid{}.Layout(gtx, 7, 6, func(gtx layout.Context, col, row int) layout.Dimensions {
				day := (row * 7) + col - startDay + 1
				if day > 0 && day <= daysInMonth {
					lbl := material.Label(mth, unit.Sp(11), fmt.Sprintf("%d", day))
					if day == now.Day() { lbl.Color = th.Primary } // Highlight Today
					return lbl.Layout(gtx)
				}
				return layout.Dimensions{}
			})
		}),
	)
}
