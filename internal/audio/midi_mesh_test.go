package audio

import (
	"testing"
)

func TestMidiMeshBroadcasting(t *testing.T) {
	cmd := MidiCommand{
		Type:     "NoteOn",
		Channel:  1,
		Note:     64,
		Velocity: 110,
	}

	// Verify BroadcastMidiEvent sends payload to mesh without panic
	BroadcastMidiEvent(cmd)

	// Verify HandleRemoteMidi processes remote note without panic
	HandleRemoteMidi("peer_synth_mesh_1", cmd)
}
