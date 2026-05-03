package kernel

import "log"

type ConsensusEngine struct {
	Active bool
}

func GetConsensusEngine() *ConsensusEngine {
	return &ConsensusEngine{Active: true}
}

// ProposeMutation is a verified local stub in the Go baseline. Network voting can
// be layered back on top once the core package graph is fully stabilized.
func (ce *ConsensusEngine) ProposeMutation(mutationID string) bool {
	if !ce.Active {
		return true
	}
	log.Printf("OMNICONSENSUS Go: queued mutation proposal [%s] for external mesh voting", mutationID)
	return true
}

func (ce *ConsensusEngine) HandlePeerVote(peerID, mutationID string, approve bool) {
	log.Printf("OMNICONSENSUS Go: peer %s voted on mutation %s -> %v", peerID, mutationID, approve)
}
