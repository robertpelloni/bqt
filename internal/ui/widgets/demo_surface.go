package widgets

import (
	"fmt"
	"image"
	"image/color"
	"strings"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/data"
	"github.com/robertpelloni/bqt/internal/kernel"
	bqtnet "github.com/robertpelloni/bqt/internal/net"
	"github.com/robertpelloni/bqt/internal/state"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// DemoSurface is the consolidated, single-page unified dashboard for BQt.
// One scrollable page, ordered by value: Audio DSP first, then Kernel/Data,
// Mesh, AI/VM, UI surfaces, and System/Tray last. Every backend subsystem in
// internal/ has a labelled control here; hover any control for a tooltip.
// There are no subpages: overlays (Dialog/Popup/Drawer) open in place.
type DemoSurface struct {
	WebView     WebView
	synthWidget *SynthWidget
	list        widget.List

	// 1: Audio DSP Engine
	btnAudioGraph, btnSynthesizer, btnFilter, btnGain, btnMasterClock widget.Clickable
	btnVoice, btnMidi, btnPlayer, btnSequencer                        widget.Clickable
	btnVolUp, btnVolDown, btnBpmUp, btnBpmDown, btnPolyToggle         widget.Clickable
	VolumeLevel                                                       float32
	ClockBpm                                                          float64
	Polyphony                                                         int

	// 2: Distributed Kernel & Data
	btnUndoStack, btnClipboard, btnTimeMachine, btnSearch, btnPermissions widget.Clickable
	btnPlugins, btnConsensus, btnSelfHeal                                 widget.Clickable
	btnGitKernel, btnMeshDB, btnPackages                                  widget.Clickable

	// 3: Mesh Network & Streaming
	btnMeshNode, btnDiscovery, btnAssetStream widget.Clickable
	btnVoiceMesh, btnMediaStream, btnHealth   widget.Clickable

	// 4: AI, VM & State
	btnAI, btnLispVM, btnMacros, btnNeural, btnValueTree widget.Clickable

	// 5: UI Interop, Widgets & WebView
	btnDialog, btnPopup, btnDrawer                            widget.Clickable
	btnTerminal, btnCalendar, btnTable, btnEditor, btnNodes   widget.Clickable
	btnTheme, btnAnimate, btnWebBack, btnWebFwd, btnWebReload widget.Clickable

	// 6: System & Tray (mirrors the taskbar tray menu)
	btnTrayMute, btnTrayMesh, btnTrayRestart, btnTrayQuit widget.Clickable
	// OnSystemAction is set by the Engine to drive the real Shell tray
	// (mute DSP, mesh toggle, EventLoop restart, quit). Nil-safe.
	OnSystemAction func(action string)

	StatusMessage string
	dialog        Dialog
	popup         Popup
	drawer        Drawer
	menu          Menu
	tooltip       ToolTip
}

func NewDemoSurface() *DemoSurface {
	ds := &DemoSurface{
		synthWidget:   NewSynthWidget(),
		StatusMessage: "BQt Unified Kernel Active | Audio DSP Ready | EventLoop Synchronized",
		VolumeLevel:   0.80,
		ClockBpm:      120.0,
		Polyphony:     8,
	}
	ds.list.Axis = layout.Vertical
	return ds
}

func (ds *DemoSurface) SetSynthVoices(poly int) {
	ds.Polyphony = poly
	if ds.synthWidget != nil {
		ds.synthWidget.SetActiveVoices(poly)
	}
}

// open is the single dispatch pattern for inspect-style controls: it explains
// the subsystem in place (Popup) and records the action in the status line.
// Parameter and navigation controls mutate real state instead of using this.
func (ds *DemoSurface) open(title, body, status string) {
	ds.popup = Popup{Visible: true, Title: title, Body: body}
	ds.StatusMessage = status
}

func (ds *DemoSurface) handleClicks(gtx layout.Context) {
	// Live parameters mutate real DemoSurface state (existing contract).
	if ds.btnVolUp.Clicked(gtx) {
		ds.VolumeLevel += 0.05
		if ds.VolumeLevel > 1.0 {
			ds.VolumeLevel = 1.0
		}
		ds.StatusMessage = fmt.Sprintf("Audio Param: Master Volume set to %.0f%%", ds.VolumeLevel*100)
	}
	if ds.btnVolDown.Clicked(gtx) {
		ds.VolumeLevel -= 0.05
		if ds.VolumeLevel < 0.0 {
			ds.VolumeLevel = 0.0
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
		switch ds.Polyphony {
		case 8:
			ds.Polyphony = 16
		case 16:
			ds.Polyphony = 4
		default:
			ds.Polyphony = 8
		}
		ds.SetSynthVoices(ds.Polyphony)
		ds.StatusMessage = fmt.Sprintf("Audio Param: Synthesizer Polyphony set to %d voices", ds.Polyphony)
	}
	// Audio DSP
	if ds.btnAudioGraph.Clicked(gtx) {
		ds.open("OmniAudioGraph", "Central DSP graph (internal/audio): native Go nodes with JUCE adapter parity.", "Audio Engine: OmniAudioGraph opened")
	}
	if ds.btnSynthesizer.Clicked(gtx) {
		ds.open("OmniSynthesizer", "Polyphonic Go synthesizer (internal/audio). Voice count follows the Voices control below.", "Audio Engine: OmniSynthesizer opened")
	}
	if ds.btnFilter.Clicked(gtx) {
		ds.open("Biquad Filter", "TPT biquad filter (lowpass/highpass/bandpass) in internal/audio. Verified by dsp_math tests.", "Audio Engine: Biquad Filter opened")
	}
	if ds.btnGain.Clicked(gtx) {
		ds.open("OmniGain", "Thread-safe gain node (internal/audio) emitting GainChanged through the unified EventLoop.", "Audio Engine: OmniGain opened")
	}
	if ds.btnMasterClock.Clicked(gtx) {
		ds.open("Master Clock", fmt.Sprintf("Temporal heartbeat at %.0f BPM, firing 16th-note steps across the graph.", ds.ClockBpm), "Audio Engine: MasterClock opened")
	}
	if ds.btnVoice.Clicked(gtx) {
		ds.open("Voice Engine", "PCM microphone capture/buffer lifecycle (internal/audio VoiceEngine). STT dispatch is not yet wired; see ROADMAP.", "Audio: VoiceEngine opened (capture ready, STT unwired)")
	}
	if ds.btnMidi.Clicked(gtx) {
		ds.open("MIDI & MidiMesh", "NoteOn/NoteOff handling and mesh broadcast (internal/audio MidiHandler/MidiMesh).", "Audio: MIDI handler opened")
	}
	if ds.btnPlayer.Clicked(gtx) {
		ds.open("Audio Player & Cluster DSP", "Playback plus DistributeDSPTask mesh offload (internal/audio). Watch for offload timeout UI in TODO.", "Audio: Player/Cluster opened")
	}
	if ds.btnSequencer.Clicked(gtx) {
		ds.open("Sequencer", "Step sequencer driven by MasterClock (internal/audio Sequencer). Tempo follows the BPM control.", "Audio: Sequencer opened")
	}
	// Kernel & Data
	if ds.btnUndoStack.Clicked(gtx) {
		u, r := kernel.GetUndoStack().Depth()
		ds.open("Undo Stack", fmt.Sprintf("History now: %d undoable, %d redoable actions (cap 500, internal/kernel).", u, r), fmt.Sprintf("Kernel Ledger: UndoStack — %d/%d", u, r))
	}
	if ds.btnClipboard.Clicked(gtx) {
		cb := kernel.GetClipboard()
		ds.open("Clipboard Sync", fmt.Sprintf("Clipboard text: %d chars. MIME types held: %v (internal/kernel Clipboard).", len(cb.GetText()), cb.MimeTypes()), "Kernel: Clipboard — live contents shown")
	}
	if ds.btnTimeMachine.Clicked(gtx) {
		// Real action: commit a dashboard snapshot; reports DB/broadcast result honestly.
		err := data.GetTimeMachine().CommitState("dashboard/status", ds.StatusMessage)
		if err != nil {
			ds.open("Time Machine", fmt.Sprintf("Snapshot FAILED: %v (database not connected in this baseline).", err), "Kernel Ledger: TimeMachine snapshot failed — see popup")
		} else {
			ds.open("Time Machine", "Snapshot committed and broadcast to mesh peers (0 peers is normal standalone). Restore/time-travel UI is future work.", "Kernel Ledger: TimeMachine snapshot committed")
		}
	}
	if ds.btnSearch.Clicked(gtx) {
		res := kernel.GetSearchEngine().GlobalSearch("bqt")
		ds.open("Omni Search", fmt.Sprintf("Query 'bqt' returned %d result(s). NOTE: local index is a seeded stub hit until real indexing lands (internal/kernel SearchEngine).", len(res)), fmt.Sprintf("Search: %d result(s) (seeded index)", len(res)))
	}
	if ds.btnPermissions.Clicked(gtx) {
		ok := kernel.GetPermissionManager().CanInteract("local-dashboard", "dashboard")
		ds.open("Permissions", fmt.Sprintf("Live check — device 'local-dashboard' may interact with 'dashboard': %v (unlocked targets allow all; internal/kernel PermissionManager).", ok), fmt.Sprintf("Security: Permissions — canInteract=%v", ok))
	}
	if ds.btnPlugins.Clicked(gtx) {
		loaded := kernel.GetPluginManager().GetLoadedPlugins()
		ds.open("Plugin Manager", fmt.Sprintf("Loaded plugins now: %d [%s]. Registry rejects duplicates (internal/kernel).", len(loaded), strings.Join(loaded, ", ")), fmt.Sprintf("Kernel: PluginManager — %d loaded", len(loaded)))
	}
	if ds.btnConsensus.Clicked(gtx) {
		ok := kernel.GetConsensusEngine().ProposeMutation("dashboard-ping")
		ds.open("Consensus & Grid Compute", fmt.Sprintf("ProposeMutation('dashboard-ping') returned %v. NOTE: local stub — mesh voting is not layered on yet (internal/kernel ConsensusEngine).", ok), "Kernel: Consensus proposal queued (stub)")
	}
	if ds.btnSelfHeal.Clicked(gtx) {
		ds.open("Self Healer, Actions & A11y", fmt.Sprintf("Registered actions: %d. Accessible elements: %d (UpdateTree logs DOM size but does not yet populate elements). SelfHealer intercepts crashes in main (internal/kernel).", kernel.GetActionRegistry().Count(), kernel.GetAccessibilityManager().ElementCount()), "Kernel: SelfHealer/Actions/A11y — live counts shown")
	}
	if ds.btnGitKernel.Clicked(gtx) {
		ds.open("Git Kernel", "In-kernel git/history services (internal/data GitKernel, HistoryReconciler).", "Data: GitKernel opened")
	}
	if ds.btnMeshDB.Clicked(gtx) {
		ds.open("Mesh DB & VFS", fmt.Sprintf("VFS assets mounted: %d. Pak archive files indexed: %d. Replicated DB: internal/data DistributedDB.", data.GetVFS().AssetCount(), data.GetPakManager().FileCount()), "Data: Mesh DB/VFS — live counts shown")
	}
	if ds.btnPackages.Clicked(gtx) {
		ds.open("Package Registry", "Mesh package registry (internal/data PackageRegistry).", "Data: PackageRegistry opened")
	}
	// Mesh & Streaming
	if ds.btnMeshNode.Clicked(gtx) {
		ds.open("Mesh Node", "Peer-to-peer mesh node; the running app starts it on port 8081 (internal/net MeshNode).", "Network: MeshNode opened")
	}
	if ds.btnDiscovery.Clicked(gtx) {
		b := bqtnet.GetDiscoveryBeacon()
		b.StartBroadcasting(8081)
		ds.open("P2P Discovery", fmt.Sprintf("Beacon broadcasting on UDP 9999: %v (announces OMNI_NODE:8081 every 5s). Mesh peers connected: %d. Stop via tray mesh toggle design in TODO.", b.IsBroadcasting(), bqtnet.GetMeshNode().PeerCount()), fmt.Sprintf("Network: Discovery broadcasting=%v", b.IsBroadcasting()))
	}
	if ds.btnAssetStream.Clicked(gtx) {
		ds.open("Asset Stream", "Chunked binary asset streaming to the mesh (internal/net StreamAssetToMesh).", "Network: Asset stream opened")
	}
	if ds.btnVoiceMesh.Clicked(gtx) {
		ds.open("Voice/Video/Desktop Streams", "Realtime voice, video, desktop and teleport streams (internal/net streaming protocols).", "Network: Media streams opened")
	}
	if ds.btnMediaStream.Clicked(gtx) {
		ds.open("Shader Sync & Live Reload", "P2P shader-skin sync and hot reload watchers (internal/ui ShaderSync/LiveReloader).", "Network: ShaderSync opened")
	}
	if ds.btnHealth.Clicked(gtx) {
		// Real readout: live peer count from the running MeshNode singleton.
		ds.open("Node Health", fmt.Sprintf("Mesh peers connected now: %d (app node on :8081). Telemetry broadcast: internal/net NodeHealth.", bqtnet.GetMeshNode().PeerCount()), fmt.Sprintf("Network: NodeHealth — %d peer(s) connected", bqtnet.GetMeshNode().PeerCount()))
	}
	// AI, VM & State
	if ds.btnAI.Clicked(gtx) {
		ds.open("AI Assistant", "Completion/repair assistant used by the SelfHealer (internal/vm AIAssistant).", "AI: Assistant opened")
	}
	if ds.btnLispVM.Clicked(gtx) {
		ds.open("Lisp VM", "Embedded Lisp evaluator and VM (internal/vm).", "VM: Lisp VM opened")
	}
	if ds.btnMacros.Clicked(gtx) {
		// No live readout: internal/vm imports internal/ui (cycle). Stats() exists on MacroRecorder for non-UI callers.
		ds.open("Macro Recorder", "Record/replay input macros (internal/vm MacroRecorder). Live state not shown here: vm imports ui, so widgets cannot import vm.", "VM: MacroRecorder opened (see note)")
	}
	if ds.btnNeural.Clicked(gtx) {
		ds.open("Neural Engine / DOM Capture", "Neural inference and neural DOM capture (internal/vm NeuralEngine).", "VM: NeuralEngine opened")
	}
	if ds.btnValueTree.Clicked(gtx) {
		vt := state.GetValueTree()
		ds.open("ValueTree State", fmt.Sprintf("Parameters set now: %d [%s]. Subscriptions broadcast every change (internal/state).", vt.ParameterCount(), strings.Join(vt.Keys(), ", ")), fmt.Sprintf("State: ValueTree — %d params", vt.ParameterCount()))
	}
	// UI surfaces & widgets (real overlay state)
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
	if ds.btnTerminal.Clicked(gtx) {
		ds.open("Widget Gallery: Terminal", "Terminal, Calendar, TableView, CodeEditor, NodeEditor, Plot, ParticleSystem and 30+ widgets live in internal/ui/widgets, each unit-tested.", "UI: Widget gallery opened")
	}
	if ds.btnCalendar.Clicked(gtx) {
		ds.open("Widget Gallery: Calendar", "Calendar/Table/Editor/Node widgets are layout-verified in internal/ui/widgets tests; this page surfaces them by category to avoid subpages.", "UI: Calendar/widgets opened")
	}
	if ds.btnTable.Clicked(gtx) {
		ds.open("Widget Gallery: Table", "TableView empty-state fallback and cell distribution are covered by table tests in internal/ui/widgets.", "UI: Table/widgets opened")
	}
	if ds.btnEditor.Clicked(gtx) {
		ds.open("Widget Gallery: Code Editor", "CodeEditor, DockArea, OmniStudio shell and Scene3D nodes are covered in internal/ui/widgets.", "UI: Editor/widgets opened")
	}
	if ds.btnNodes.Clicked(gtx) {
		ds.open("Widget Gallery: Node Editor", "NodeEditor, ShaderEffect, Raymarcher and RHINode procedural surfaces (internal/ui/widgets).", "UI: Node/widgets opened")
	}
	if ds.btnTheme.Clicked(gtx) {
		ds.open("Theme & Animation", "Palette generation, stylesheet parsing and cubic ease-out animation (internal/ui/theme, internal/ui/anim).", "UI: Theme/Anim opened")
	}
	if ds.btnAnimate.Clicked(gtx) {
		ds.open("Compute & Haptics", "GPU ComputeKernel dispatch, HapticsEngine proximity and LiveReloader (internal/ui).", "UI: Compute/Haptics opened")
	}
	// WebView navigation mutates the real WebView history/bridge state.
	if ds.btnWebBack.Clicked(gtx) {
		ds.WebView.Back()
		ds.StatusMessage = "WebView: Back (boundary-safe no-op at history start)"
	}
	if ds.btnWebFwd.Clicked(gtx) {
		ds.WebView.Forward()
		ds.StatusMessage = "WebView: Forward (boundary-safe no-op at history end)"
	}
	if ds.btnWebReload.Clicked(gtx) {
		ds.WebView.Navigate("about:bqt", "")
		ds.StatusMessage = "WebView: Reloaded about:bqt"
	}
	// System & Tray: drive the real shell tray through the Engine callback.
	sys := func(btn *widget.Clickable, action, label string) {
		if btn.Clicked(gtx) {
			if ds.OnSystemAction != nil {
				ds.OnSystemAction(action)
			}
			ds.StatusMessage = "System (tray): " + label + " — also in the taskbar BQt Tray menu (bottom-right)"
		}
	}
	sys(&ds.btnTrayMute, "mute", "Mute/Unmute Audio DSP")
	sys(&ds.btnTrayMesh, "mesh", "Disconnect/Reconnect P2P Mesh")
	sys(&ds.btnTrayRestart, "restart", "Restart EventLoop")
	sys(&ds.btnTrayQuit, "quit", "Quit BQt Servers (stops EventLoop; close window to exit)")
}

type tipEntry struct {
	btn  *widget.Clickable
	text string
	y    float32
}

func (ds *DemoSurface) updateTooltip() {
	tips := []tipEntry{
		{&ds.btnAudioGraph, "[AudioGraph] Central DSP graph: native Go nodes, JUCE parity.", 120},
		{&ds.btnSynthesizer, "[Synthesizer] Polyphonic synth; set voices with the Voices control.", 120},
		{&ds.btnFilter, "[Filter] TPT biquad: lowpass / highpass / bandpass.", 120},
		{&ds.btnGain, "[Gain] Thread-safe volume node with GainChanged signals.", 120},
		{&ds.btnMasterClock, "[MasterClock] 16th-note heartbeat; tempo from the BPM control.", 120},
		{&ds.btnVoice, "[Voice] Mic PCM capture. STT dispatch not yet wired.", 120},
		{&ds.btnMidi, "[MIDI] Note handling + MidiMesh broadcast.", 120},
		{&ds.btnPlayer, "[Player] Playback + cluster DSP offload to mesh peers.", 120},
		{&ds.btnSequencer, "[Sequencer] Step patterns locked to MasterClock BPM.", 120},
		{&ds.btnVolUp, "[Volume] Master gain 0-100% in 5% steps.", 150},
		{&ds.btnVolDown, "[Volume] Master gain 0-100% in 5% steps.", 150},
		{&ds.btnBpmUp, "[Tempo] MasterClock 40-240 BPM in 5 BPM steps.", 150},
		{&ds.btnBpmDown, "[Tempo] MasterClock 40-240 BPM in 5 BPM steps.", 150},
		{&ds.btnPolyToggle, "[Voices] Cycle synth polyphony 8 > 16 > 4.", 150},
		{&ds.btnUndoStack, "[UndoStack] Roll back ledgered state changes.", 220},
		{&ds.btnClipboard, "[Clipboard] P2P shared clipboard buffer.", 220},
		{&ds.btnTimeMachine, "[TimeMachine] Snapshot and restore system state.", 220},
		{&ds.btnSearch, "[Search] Query local state and mesh peers.", 220},
		{&ds.btnPermissions, "[Permissions] Capability/RBAC checks.", 220},
		{&ds.btnPlugins, "[Plugins] Registry; duplicate registration is rejected.", 220},
		{&ds.btnConsensus, "[Consensus] Peer agreement + grid compute.", 220},
		{&ds.btnSelfHeal, "[SelfHealer] Crash interception, AI repair, a11y DOM.", 220},
		{&ds.btnGitKernel, "[GitKernel] Git/history services in internal/data.", 260},
		{&ds.btnMeshDB, "[MeshDB/VFS] Replicated DB, Pak archives, image VFS.", 260},
		{&ds.btnPackages, "[Packages] Mesh package registry.", 260},
		{&ds.btnMeshNode, "[MeshNode] P2P node started by the app on :8081.", 300},
		{&ds.btnDiscovery, "[Discovery] Multicast peer discovery heartbeat.", 300},
		{&ds.btnAssetStream, "[AssetStream] Chunked binary streaming to peers.", 300},
		{&ds.btnVoiceMesh, "[Streams] Voice, video, desktop, teleport streams.", 300},
		{&ds.btnMediaStream, "[ShaderSync] P2P shader sync + live reload.", 300},
		{&ds.btnHealth, "[NodeHealth] Peer telemetry broadcast.", 300},
		{&ds.btnAI, "[AI] Assistant behind SelfHealer repair reports.", 340},
		{&ds.btnLispVM, "[LispVM] Embedded Lisp evaluator.", 340},
		{&ds.btnMacros, "[Macros] Record and replay input macros.", 340},
		{&ds.btnNeural, "[Neural] Inference + neural DOM capture.", 340},
		{&ds.btnValueTree, "[ValueTree] Hierarchical params with subscriptions.", 340},
		{&ds.btnDialog, "[Dialog] Modal dialog with scrim. Opens in place.", 380},
		{&ds.btnPopup, "[Popup] Transient in-place feedback card.", 380},
		{&ds.btnDrawer, "[Drawer] Slide-out navigation drawer.", 380},
		{&ds.btnTerminal, "[Widgets] Terminal + 30 tested widgets, one gallery entry each.", 380},
		{&ds.btnTheme, "[Theme] Palettes, stylesheets, animation easing.", 380},
		{&ds.btnWebBack, "[WebView] History back; safe no-op at the start.", 400},
		{&ds.btnWebFwd, "[WebView] History forward; safe no-op at the end.", 400},
		{&ds.btnWebReload, "[WebView] Reload about:bqt in the embedded bridge.", 400},
		{&ds.btnTrayQuit, "[Quit] Stops the EventLoop via the tray. Close the window to fully exit.", 440},
	}
	ds.tooltip.Visible = false
	for _, t := range tips {
		if t.btn.Hovered() {
			ds.tooltip = ToolTip{Visible: true, Text: t.text, X: 120, Y: int(t.y)}
			return
		}
	}
}

func btnRow(mth *material.Theme, pairs ...interface{}) layout.Widget {
	return func(gtx layout.Context) layout.Dimensions {
		var kids []layout.FlexChild
		for i := 0; i+1 < len(pairs); i += 2 {
			btn := pairs[i].(*widget.Clickable)
			label := pairs[i+1].(string)
			b, l := btn, label
			kids = append(kids, layout.Rigid(material.Button(mth, b, l).Layout))
		}
		return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceStart}.Layout(gtx, kids...)
	}
}

func (ds *DemoSurface) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Primary
	ds.handleClicks(gtx)
	ds.updateTooltip()

	sections := []layout.Widget{
		func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					h := material.H6(mth, "BQt Unified Dashboard — one page, scroll for all systems")
					h.Color = th.Text
					return h.Layout(gtx)
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					st := material.Caption(mth, "Status: "+ds.StatusMessage+"  |  Start at Login > BOOT KERNEL. Tray: taskbar bottom-right. Hover any button for help (?)")
					st.Color = th.Primary
					return st.Layout(gtx)
				}),
			)
		},
		func(gtx layout.Context) layout.Dimensions {
			return ds.renderPanel(gtx, mth, th, "1 · Audio DSP Engine — start here (graph, synth, filter, gain, clock, voice, MIDI)", btnRow(mth,
				&ds.btnAudioGraph, "AudioGraph", &ds.btnSynthesizer, "Synthesizer", &ds.btnFilter, "Filter", &ds.btnGain, "Gain", &ds.btnMasterClock, "Clock",
				&ds.btnVoice, "Voice", &ds.btnMidi, "MIDI", &ds.btnPlayer, "Player", &ds.btnSequencer, "Sequencer"))
		},
		func(gtx layout.Context) layout.Dimensions {
			return ds.renderPanel(gtx, mth, th, "1b · Live Parameters — these change real engine state", func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Horizontal, Alignment: layout.Middle}.Layout(gtx,
					layout.Rigid(material.Caption(mth, fmt.Sprintf("Vol %.0f%% ", ds.VolumeLevel*100)).Layout),
					layout.Rigid(material.Button(mth, &ds.btnVolDown, "Vol -").Layout),
					layout.Rigid(material.Button(mth, &ds.btnVolUp, "Vol +").Layout),
					layout.Rigid(material.Caption(mth, fmt.Sprintf("  BPM %.0f ", ds.ClockBpm)).Layout),
					layout.Rigid(material.Button(mth, &ds.btnBpmDown, "BPM -").Layout),
					layout.Rigid(material.Button(mth, &ds.btnBpmUp, "BPM +").Layout),
					layout.Rigid(material.Button(mth, &ds.btnPolyToggle, fmt.Sprintf("Voices: %d", ds.Polyphony)).Layout),
				)
			})
		},
		func(gtx layout.Context) layout.Dimensions {
			if ds.synthWidget == nil {
				return layout.Dimensions{}
			}
			return layout.Stack{}.Layout(gtx, layout.Stacked(func(gtx layout.Context) layout.Dimensions {
				gtx.Constraints.Max.Y = 90
				gtx.Constraints.Min.Y = 90
				return ds.synthWidget.Layout(gtx)
			}))
		},
		func(gtx layout.Context) layout.Dimensions {
			return ds.renderPanel(gtx, mth, th, "2 · Kernel, Ledger & Data — undo, search, security, plugins, git, storage", btnRow(mth,
				&ds.btnUndoStack, "Undo", &ds.btnClipboard, "Clipboard", &ds.btnTimeMachine, "TimeMachine", &ds.btnSearch, "Search", &ds.btnPermissions, "Permissions",
				&ds.btnPlugins, "Plugins", &ds.btnConsensus, "Consensus", &ds.btnSelfHeal, "Self-Heal", &ds.btnGitKernel, "Git", &ds.btnMeshDB, "MeshDB/VFS", &ds.btnPackages, "Packages"))
		},
		func(gtx layout.Context) layout.Dimensions {
			return ds.renderPanel(gtx, mth, th, "3 · Mesh Network & Streaming — peers, discovery, assets, media, health", btnRow(mth,
				&ds.btnMeshNode, "Mesh Node", &ds.btnDiscovery, "Discovery", &ds.btnAssetStream, "Assets", &ds.btnVoiceMesh, "Voice/Video", &ds.btnMediaStream, "ShaderSync", &ds.btnHealth, "Health"))
		},
		func(gtx layout.Context) layout.Dimensions {
			return ds.renderPanel(gtx, mth, th, "4 · AI, VM & State — assistant, Lisp, macros, neural, ValueTree", btnRow(mth,
				&ds.btnAI, "AI Assistant", &ds.btnLispVM, "Lisp VM", &ds.btnMacros, "Macros", &ds.btnNeural, "Neural", &ds.btnValueTree, "ValueTree"))
		},
		func(gtx layout.Context) layout.Dimensions {
			return ds.renderPanel(gtx, mth, th, "5 · UI Surfaces & Widgets — overlays open in place; gallery covers all 30+ widgets", btnRow(mth,
				&ds.btnDialog, "Dialog", &ds.btnPopup, "Popup", &ds.btnDrawer, "Drawer", &ds.btnTerminal, "Terminal+", &ds.btnCalendar, "Calendar+", &ds.btnTable, "Table+", &ds.btnEditor, "Editor+", &ds.btnNodes, "Nodes+", &ds.btnTheme, "Theme/Anim", &ds.btnAnimate, "Compute/Haptics"))
		},
		func(gtx layout.Context) layout.Dimensions {
			return ds.renderPanel(gtx, mth, th, fmt.Sprintf("5b · WebView — bridge queue: %d (Back/Forward are boundary-safe)", ds.WebView.QueueDepth()), func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Horizontal, Alignment: layout.Middle}.Layout(gtx,
					layout.Rigid(material.Button(mth, &ds.btnWebBack, "< Back").Layout),
					layout.Rigid(material.Button(mth, &ds.btnWebFwd, "Forward >").Layout),
					layout.Rigid(material.Button(mth, &ds.btnWebReload, "Reload").Layout),
					layout.Rigid(material.Caption(mth, "  Embedded WebView bridge; navigation is logged by the Engine.").Layout),
				)
			})
		},
		func(gtx layout.Context) layout.Dimensions {
			return ds.renderPanel(gtx, mth, th, "6 · System & Tray — same actions as the taskbar tray menu (bottom-right)", btnRow(mth,
				&ds.btnTrayMute, "Mute DSP", &ds.btnTrayMesh, "Mesh On/Off", &ds.btnTrayRestart, "Restart Loop", &ds.btnTrayQuit, "Quit Servers"))
		},
	}

	return layout.Stack{}.Layout(gtx,
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return layout.UniformInset(unit.Dp(12)).Layout(gtx, func(gtx layout.Context) layout.Dimensions {
				return material.List(mth, &ds.list).Layout(gtx, len(sections), func(gtx layout.Context, i int) layout.Dimensions {
					return layout.Inset{Bottom: unit.Dp(8)}.Layout(gtx, sections[i])
				})
			})
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dims, _ := ds.dialog.Layout(gtx, th)
			return dims
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions { return ds.popup.Layout(gtx, th) }),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return ds.drawer.Layout(gtx, th, func(gtx layout.Context) layout.Dimensions {
				return material.Body1(mth, "Navigation Drawer Content — all sections live on the main page; this drawer is a widget demo.").Layout(gtx)
			})
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions { return ds.tooltip.Layout(gtx, th) }),
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
