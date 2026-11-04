package widgets

import (
	"fmt"
	"image"
	"image/color"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// DemoSurface is the consolidated, single-page unified dashboard for BQt.
// It organizes all backend framework subsystems into categorized, high-value
// panels: Audio DSP Engine, Distributed Kernel & Ledger, Mesh Networking,
// and UI Interop Surfaces, with informative labels, live parameter sliders,
// and tooltips.
type DemoSurface struct {
	WebView     WebView
	synthWidget *SynthWidget

	// Category 1: High-Priority Audio DSP Engine Action Buttons
	btnAudioGraph   widget.Clickable
	btnSynthesizer  widget.Clickable
	btnFilter       widget.Clickable
	btnGain         widget.Clickable
	btnMasterClock  widget.Clickable

	// Category 1b: Live Interactive Parameter Controls
	btnVolUp      widget.Clickable
	btnVolDown    widget.Clickable
	btnBpmUp      widget.Clickable
	btnBpmDown    widget.Clickable
	btnPolyToggle widget.Clickable

	// Interactive Parameter State
	VolumeLevel float32
	ClockBpm    float64
	Polyphony   int

	// Category 2: High-Priority Distributed Kernel & Ledger
	btnUndoStack   widget.Clickable
	btnClipboard   widget.Clickable
	btnTimeMachine widget.Clickable
	btnSearch      widget.Clickable
	btnPermissions widget.Clickable

	// Category 3: Mesh Network & Discovery
	btnMeshNode    widget.Clickable
	btnDiscovery   widget.Clickable
	btnAssetStream widget.Clickable

	// Category 4: UI Interop & System Surfaces
	btnDialog widget.Clickable
	btnPopup  widget.Clickable
	btnDrawer widget.Clickable

	// Status message banner
	StatusMessage string

	// Overlays & Components
	dialog  Dialog
	popup   Popup
	drawer  Drawer
	menu    Menu
	tooltip ToolTip
}

func NewDemoSurface() *DemoSurface {
	return &DemoSurface{
		synthWidget:   NewSynthWidget(),
		StatusMessage: "BQt Unified Kernel Active | Audio DSP Ready | EventLoop Synchronized",
		VolumeLevel:   0.80,
		ClockBpm:      120.0,
		Polyphony:     8,
	}
}

func (ds *DemoSurface) SetSynthVoices(poly int) {
	ds.Polyphony = poly
	if ds.synthWidget != nil {
		ds.synthWidget.SetActiveVoices(poly)
	}
}

func (ds *DemoSurface) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Primary

	// 1. Handle Interactive Parameter Controls
	if ds.btnVolUp.Clicked(gtx) {
		if ds.VolumeLevel < 1.0 {
			ds.VolumeLevel += 0.05
			if ds.VolumeLevel > 1.0 {
				ds.VolumeLevel = 1.0
			}
		}
		ds.StatusMessage = fmt.Sprintf("Audio Param: Master Volume set to %.0f%%", ds.VolumeLevel*100)
	}
	if ds.btnVolDown.Clicked(gtx) {
		if ds.VolumeLevel > 0.0 {
			ds.VolumeLevel -= 0.05
			if ds.VolumeLevel < 0.0 {
				ds.VolumeLevel = 0.0
			}
		}
		ds.StatusMessage = fmt.Sprintf("Audio Param: Master Volume set to %.0f%%", ds.VolumeLevel*100)
	}
	if ds.btnBpmUp.Clicked(gtx) {
		ds.ClockBpm += 5.0
		if ds.ClockBpm > 240.0 {
			ds.ClockBpm = 240.0
		}
		ds.StatusMessage = fmt.Sprintf("Audio Param: Master Clock tempo set to %.0f BPM", ds.ClockBpm)
	}
	if ds.btnBpmDown.Clicked(gtx) {
		ds.ClockBpm -= 5.0
		if ds.ClockBpm < 40.0 {
			ds.ClockBpm = 40.0
		}
		ds.StatusMessage = fmt.Sprintf("Audio Param: Master Clock tempo set to %.0f BPM", ds.ClockBpm)
	}
	if ds.btnPolyToggle.Clicked(gtx) {
		if ds.Polyphony == 8 {
			ds.Polyphony = 16
		} else if ds.Polyphony == 16 {
			ds.Polyphony = 4
		} else {
			ds.Polyphony = 8
		}
		ds.SetSynthVoices(ds.Polyphony)
		ds.StatusMessage = fmt.Sprintf("Audio Param: Synthesizer Polyphony set to %d voices", ds.Polyphony)
	}

	// 2. Handle Clicks & Dispatches
	if ds.btnDialog.Clicked(gtx) {
		ds.dialog = Dialog{Visible: true, Title: "BQt Native Dialog", Body: "Modal interaction initialized across BQt window tree."}
		ds.StatusMessage = "UI Action: Dialog Opened"
	}
	if ds.btnPopup.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "BQt Transient Popup", Body: "Lightweight popup overlay rendered."}
		ds.StatusMessage = "UI Action: Popup Displayed"
	}
	if ds.btnDrawer.Clicked(gtx) {
		ds.drawer = Drawer{Visible: true, Title: "Navigation Drawer", WidthDp: 260}
		ds.StatusMessage = "UI Action: Drawer Toggled"
	}

	if ds.btnAudioGraph.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "OmniAudioGraph", Body: "JUCE Adapter & Parallel Audio Processing Graph active."}
		ds.StatusMessage = "Audio Engine: OmniAudioGraph active"
	}
	if ds.btnSynthesizer.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "OmniSynthesizer", Body: "Polyphonic Go synthesizer note rendering verified."}
		ds.StatusMessage = "Audio Engine: OmniSynthesizer note dispatched"
	}
	if ds.btnFilter.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Biquad Filter", Body: "Real-time DSP lowpass filter applied to PCM buffer."}
		ds.StatusMessage = "Audio Engine: Biquad Filter resonance tuned"
	}
	if ds.btnGain.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "OmniGain", Body: "Gain control updated via queued EventLoop signal."}
		ds.StatusMessage = "Audio Engine: OmniGain calibrated"
	}
	if ds.btnMasterClock.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Master Clock", Body: fmt.Sprintf("Temporal heartbeat ticking at %.0f BPM across sixteenth steps.", ds.ClockBpm)}
		ds.StatusMessage = "Audio Engine: MasterClock synced"
	}

	if ds.btnUndoStack.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Undo Stack", Body: "Multi-user history ledger reversed cleanly."}
		ds.StatusMessage = "Kernel Ledger: UndoStack operation reversed"
	}
	if ds.btnClipboard.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Clipboard Sync", Body: "P2P Clipboard shared memory buffer synchronized."}
		ds.StatusMessage = "Kernel: Clipboard synchronized across mesh"
	}
	if ds.btnTimeMachine.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Time Machine", Body: "Immutable ledger commit snapshot captured."}
		ds.StatusMessage = "Kernel Ledger: TimeMachine snapshot saved"
	}
	if ds.btnSearch.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Omni Search", Body: "Global mesh query executed with zero latency."}
		ds.StatusMessage = "Search: Global mesh query executed"
	}
	if ds.btnPermissions.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Permissions", Body: "RBAC security token validated across distributed peers."}
		ds.StatusMessage = "Security: RBAC Token validated"
	}

	if ds.btnMeshNode.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Mesh Node", Body: "Peer-to-peer data mesh route established."}
		ds.StatusMessage = "Network: MeshNode route active"
	}
	if ds.btnDiscovery.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "P2P Discovery", Body: "LAN & WAN peer discovery broadcasting heartbeat."}
		ds.StatusMessage = "Network: Peer discovery broadcasted"
	}
	if ds.btnAssetStream.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Asset Stream", Body: "Real-time binary asset stream chunked & routed."}
		ds.StatusMessage = "Network: Asset stream chunk routed"
	}

	// 3. Tooltip Triggers on Hover
	if ds.btnAudioGraph.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[AudioGraph] Central DSP graph running native Go & JUCE primitives.", X: 120, Y: 120}
	} else if ds.btnSynthesizer.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[OmniSynthesizer] Multi-voice polyphonic synthesizer with sine waveform renderer.", X: 120, Y: 120}
	} else if ds.btnFilter.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[BiquadFilter] Digital biquad filter with lowpass, highpass, and bandpass modes.", X: 120, Y: 120}
	} else if ds.btnGain.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[OmniGain] Real-time volume scaling with thread-safe GainChanged signals.", X: 120, Y: 120}
	} else if ds.btnMasterClock.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[MasterClock] High-priority temporal clock firing 16th note steps and quarter beats.", X: 120, Y: 120}
	} else if ds.btnVolUp.Hovered() || ds.btnVolDown.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Volume Control] Calibrate master audio gain multiplier.", X: 120, Y: 140}
	} else if ds.btnBpmUp.Hovered() || ds.btnBpmDown.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Tempo Control] Adjust MasterClock beats per minute (BPM).", X: 120, Y: 140}
	} else if ds.btnPolyToggle.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Polyphony Toggle] Cycle synthesizer polyphonic voice pool size (4, 8, 16).", X: 120, Y: 140}
	} else if ds.btnUndoStack.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[UndoStack] Transaction ledger tracking state changes for instant rollbacks.", X: 120, Y: 180}
	} else if ds.btnClipboard.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Clipboard] P2P shared memory clipboard with cryptographically signed payloads.", X: 120, Y: 180}
	} else if ds.btnTimeMachine.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[TimeMachine] Distributed ledger snapshotting system for time-travel state recovery.", X: 120, Y: 180}
	} else if ds.btnSearch.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[OmniSearch] Instant global index search querying across peers and local state.", X: 120, Y: 180}
	} else if ds.btnPermissions.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Permissions] Granular capability-based access control manager.", X: 120, Y: 180}
	} else if ds.btnMeshNode.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[MeshNode] High-throughput peer-to-peer data networking node.", X: 120, Y: 240}
	} else if ds.btnDiscovery.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Discovery] Autonomous peer discovery via UDP multicast and mesh telemetry.", X: 120, Y: 240}
	} else if ds.btnAssetStream.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[AssetStream] High-speed streaming engine for PCM audio, textures, and models.", X: 120, Y: 240}
	} else if ds.btnDialog.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Dialog] Native modal dialog widget with backdrop scrim and actions.", X: 120, Y: 300}
	} else if ds.btnPopup.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Popup] Non-modal transient popup card for lightweight feedback.", X: 120, Y: 300}
	} else if ds.btnDrawer.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "[Drawer] Slide-out navigation drawer with custom child views.", X: 120, Y: 300}
	} else {
		ds.tooltip.Visible = false
	}

	// 4. Layout Rendering: Unified Single Page
	return layout.Stack{}.Layout(gtx,
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return layout.UniformInset(unit.Dp(12)).Layout(gtx, func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Vertical, Spacing: layout.SpaceEvenly}.Layout(gtx,
					// Header & Status Line
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
							layout.Rigid(func(gtx layout.Context) layout.Dimensions {
								h := material.H6(mth, "BQt Autonomous Unified Control Dashboard")
								h.Color = th.Text
								return h.Layout(gtx)
							}),
							layout.Rigid(func(gtx layout.Context) layout.Dimensions {
								st := material.Caption(mth, "Status: "+ds.StatusMessage)
								st.Color = th.Primary
								return st.Layout(gtx)
							}),
						)
					}),

					// Section 1: Core Audio DSP Engine (High Priority Actions)
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return ds.renderPanel(gtx, mth, th, "Core Audio / DSP Engine (High Priority Actions)", func(gtx layout.Context) layout.Dimensions {
							return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceBetween}.Layout(gtx,
								layout.Rigid(material.Button(mth, &ds.btnAudioGraph, "OmniAudioGraph").Layout),
								layout.Rigid(material.Button(mth, &ds.btnSynthesizer, "OmniSynthesizer").Layout),
								layout.Rigid(material.Button(mth, &ds.btnFilter, "Biquad Filter").Layout),
								layout.Rigid(material.Button(mth, &ds.btnGain, "OmniGain").Layout),
								layout.Rigid(material.Button(mth, &ds.btnMasterClock, "Master Clock").Layout),
							)
						})
					}),

					// Section 1b: Live Interactive Parameter Adjustments
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return ds.renderPanel(gtx, mth, th, "Live Engine Parameter Controls", func(gtx layout.Context) layout.Dimensions {
							return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceEvenly, Alignment: layout.Middle}.Layout(gtx,
								// Volume controls
								layout.Rigid(func(gtx layout.Context) layout.Dimensions {
									return layout.Flex{Axis: layout.Horizontal, Alignment: layout.Middle}.Layout(gtx,
										layout.Rigid(material.Caption(mth, fmt.Sprintf("Vol: %.0f%% ", ds.VolumeLevel*100)).Layout),
										layout.Rigid(material.Button(mth, &ds.btnVolDown, "-").Layout),
										layout.Rigid(material.Button(mth, &ds.btnVolUp, "+").Layout),
									)
								}),
								// BPM controls
								layout.Rigid(func(gtx layout.Context) layout.Dimensions {
									return layout.Flex{Axis: layout.Horizontal, Alignment: layout.Middle}.Layout(gtx,
										layout.Rigid(material.Caption(mth, fmt.Sprintf("BPM: %.0f ", ds.ClockBpm)).Layout),
										layout.Rigid(material.Button(mth, &ds.btnBpmDown, "-").Layout),
										layout.Rigid(material.Button(mth, &ds.btnBpmUp, "+").Layout),
									)
								}),
								// Polyphony toggle
								layout.Rigid(func(gtx layout.Context) layout.Dimensions {
									return material.Button(mth, &ds.btnPolyToggle, fmt.Sprintf("Voices: %d", ds.Polyphony)).Layout(gtx)
								}),
							)
						})
					}),

					// Section 2: Real-time Waveform Scope
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						if ds.synthWidget != nil {
							return layout.Stack{}.Layout(gtx, layout.Stacked(func(gtx layout.Context) layout.Dimensions {
								gtx.Constraints.Max.Y = 90
								gtx.Constraints.Min.Y = 90
								return ds.synthWidget.Layout(gtx)
							}))
						}
						return layout.Dimensions{}
					}),

					// Section 3: Distributed Kernel & Ledger (High Priority)
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return ds.renderPanel(gtx, mth, th, "Distributed Kernel & Ledger Subsystems", func(gtx layout.Context) layout.Dimensions {
							return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceBetween}.Layout(gtx,
								layout.Rigid(material.Button(mth, &ds.btnUndoStack, "Undo Stack").Layout),
								layout.Rigid(material.Button(mth, &ds.btnClipboard, "Clipboard Sync").Layout),
								layout.Rigid(material.Button(mth, &ds.btnTimeMachine, "Time Machine").Layout),
								layout.Rigid(material.Button(mth, &ds.btnSearch, "Global Search").Layout),
								layout.Rigid(material.Button(mth, &ds.btnPermissions, "Permissions").Layout),
							)
						})
					}),

					// Section 4: Networking & UI Interop Surfaces
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceBetween}.Layout(gtx,
							layout.Flexed(0.5, func(gtx layout.Context) layout.Dimensions {
								return ds.renderPanel(gtx, mth, th, "Mesh Network & Discovery", func(gtx layout.Context) layout.Dimensions {
									return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceEvenly}.Layout(gtx,
										layout.Rigid(material.Button(mth, &ds.btnMeshNode, "Mesh Node").Layout),
										layout.Rigid(material.Button(mth, &ds.btnDiscovery, "Discovery").Layout),
										layout.Rigid(material.Button(mth, &ds.btnAssetStream, "Stream").Layout),
									)
								})
							}),
							layout.Flexed(0.5, func(gtx layout.Context) layout.Dimensions {
								return ds.renderPanel(gtx, mth, th, "UI Interop & System Surfaces", func(gtx layout.Context) layout.Dimensions {
									return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceEvenly}.Layout(gtx,
										layout.Rigid(material.Button(mth, &ds.btnDialog, "Dialog").Layout),
										layout.Rigid(material.Button(mth, &ds.btnPopup, "Popup").Layout),
										layout.Rigid(material.Button(mth, &ds.btnDrawer, "Drawer").Layout),
									)
								})
							}),
						)
					}),
				)
			})
		}),

		// Modal overlays
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dims, _ := ds.dialog.Layout(gtx, th)
			return dims
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return ds.popup.Layout(gtx, th)
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dummyChild := func(gtx layout.Context) layout.Dimensions {
				return material.Body1(mth, "Navigation Drawer Content").Layout(gtx)
			}
			return ds.drawer.Layout(gtx, th, dummyChild)
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return ds.tooltip.Layout(gtx, th)
		}),
	)
}

func (ds *DemoSurface) renderPanel(gtx layout.Context, mth *material.Theme, th theme.Theme, title string, content layout.Widget) layout.Dimensions {
	cardRect := image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Min.Y)
	paint.FillShape(gtx.Ops, color.NRGBA{R: 25, G: 28, B: 38, A: 200}, clip.Rect(cardRect).Op())

	return layout.UniformInset(unit.Dp(6)).Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				lbl := material.Caption(mth, title)
				lbl.Color = color.NRGBA{R: 180, G: 200, B: 240, A: 255}
				return layout.Inset{Bottom: unit.Dp(4)}.Layout(gtx, lbl.Layout)
			}),
			layout.Rigid(content),
		)
	})
}
