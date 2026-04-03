package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type OmniStudio struct {
	Editor    *CodeEditor
	Terminal  *Terminal
	SplitView *SplitView
}

func NewOmniStudio() *OmniStudio {
	return &OmniStudio{
		Editor:    &CodeEditor{},
		Terminal:  &Terminal{},
		SplitView: &SplitView{Ratio: 0.7},
	}
}

func (os *OmniStudio) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- OMNISTUDIO MULTIPLAYER IDE RENDER PASS ---
	// Unifying Logic, Shell, and GPU Rendering into a single workspace.
	
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	return os.SplitView.Layout(gtx,
		func(gtx layout.Context) layout.Dimensions {
			// Center: Collaborative Code Editor
			return os.Editor.Layout(gtx, th)
		},
		func(gtx layout.Context) layout.Dimensions {
			// Sidebar: Integrated Native Shell
			return os.Terminal.Layout(gtx, th)
		},
	)
}
