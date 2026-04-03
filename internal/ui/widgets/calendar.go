package widgets

import (
	"fmt"
	"time"
	"gioui.org/layout"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Calendar struct {
	ViewDate time.Time
	PrevBtn, NextBtn widget.Clickable
}

func (c *Calendar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if c.ViewDate.IsZero() { c.ViewDate = time.Now() }

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	// --- GO TEMPORAL NAVIGATION ---
	if c.PrevBtn.Clicked(gtx) { c.ViewDate = c.ViewDate.AddDate(0, -1, 0) }
	if c.NextBtn.Clicked(gtx) { c.ViewDate = c.ViewDate.AddDate(0, 1, 0) }

	first := time.Date(c.ViewDate.Year(), c.ViewDate.Month(), 1, 0, 0, 0, 0, time.Local)
	startDay := int(first.Weekday())

	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Spacing: layout.SpaceBetween}.Layout(gtx,
				layout.Rigid(material.Button(mth, &c.PrevBtn, "<").Layout),
				layout.Rigid(material.H6(mth, fmt.Sprintf("%s %d", c.ViewDate.Month(), c.ViewDate.Year())).Layout),
				layout.Rigid(material.Button(mth, &c.NextBtn, ">").Layout),
			)
		}),
		layout.Rigid(layout.Spacer{Height: unit.Dp(10)}.Layout),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			// Render 7x6 High-Fidelity Grid
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
