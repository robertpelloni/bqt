package widgets

import (
	"time"
	"fmt"
	"gioui.org/layout"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Calendar struct {
	ActiveDate time.Time
}

func (c *Calendar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	now := time.Now()
	firstOfMonth := time.Date(now.Year(), now.Month(), 1, 0, 0, 0, 0, time.Local)
	startDay := int(firstOfMonth.Weekday())

	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.H6(mth, fmt.Sprintf("%s %d", now.Month(), now.Year())).Layout(gtx)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			// --- GO CALENDAR GRID RENDER PASS ---
			// Calculate and render 7x6 grid for the current month
			return layout.Grid{}.Layout(gtx, 7, 6, func(gtx layout.Context, col, row int) layout.Dimensions {
				day := (row * 7) + col - startDay + 1
				if day > 0 && day <= 31 {
					return material.Label(mth, unit.Sp(10), fmt.Sprintf("%d", day)).Layout(gtx)
				}
				return layout.Dimensions{}
			})
		}),
	)
}
