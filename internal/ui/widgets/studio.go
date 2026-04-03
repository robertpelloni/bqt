package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/vm"
)

type OmniStudio struct {
	Editor    *CodeEditor
	Terminal  *Terminal
	Explorer  *MeshExplorer
	SplitView *SplitView
}

func (os *OmniStudio) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	// --- GO AI SUGGESTION RENDER ---
	assistant := vm.GetAIAssistant()
	suggestion := assistant.GetLatestSuggestion()
	if suggestion.Text != "" {
		// Render Ghost-Text in Aetheria Gold
		lbl := material.Label(mth, unit.Sp(12), suggestion.Text)
		lbl.Color = th.Primary // Gold
		// ... (positioning at cursor)
	}

	return os.SplitView.Layout(gtx,
		func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return os.Explorer.Layout(gtx, th)
				}),
				layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
					return os.Editor.Layout(gtx, th)
				}),
			)
		},
		func(gtx layout.Context) layout.Dimensions {
			return os.Terminal.Layout(gtx, th)
		},
	)
}
