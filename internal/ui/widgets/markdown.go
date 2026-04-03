package widgets

import (
	"strings"
	"gioui.org/layout"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type MarkdownView struct {
	Text string
}

func (m *MarkdownView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// Native Go Markdown Renderer
	lines := strings.Split(m.Text, "\n")
	
	list := &layout.List{Axis: layout.Vertical}
	return list.Layout(gtx, len(lines), func(gtx layout.Context, i int) layout.Dimensions {
		line := lines[i]
		
		// Simple Tokenizer Logic (Go Port)
		size := unit.Sp(14)
		if strings.HasPrefix(line, "# ") {
			size = unit.Sp(24)
			line = line[2:]
		} else if strings.HasPrefix(line, "## ") {
			size = unit.Sp(20)
			line = line[3:]
		}

		lbl := material.Label(material.NewTheme(), size, line)
		lbl.Color = th.Text
		return lbl.Layout(gtx)
	})
}
