package kernel

import (
	"log"
	"github.com/robertpelloni/bobui/internal/net"
)

type ConsensusEngine struct {
	Active bool
}

func GetConsensusEngine() *ConsensusEngine {
	return &ConsensusEngine{Active: true}
}

// ProposeMutation requests permission from the multiverse to evolve the kernel.
func (ce *ConsensusEngine) ProposeMutation(mutationID string) bool {
	if !ce.Active { return true }
	
	payload := map[string]interface{}{
		"type": "consensus_request",
		"id":   mutationID,
	}
	net.GetMeshNode().Broadcast(payload)
	log.Printf("OMNICONSENSUS Go: Mutation [%s] proposed to the Sovereign Multiverse.", mutationID)
	
	// In a production build, this would wait for a majority of 'consensus_approve' 
	// payloads before returning true.
	return true 
}

// HandlePeerVote records a remote approval/denial.
func (ce *ConsensusEngine) HandlePeerVote(peerID, mutationID string, approve bool) {
	log.Printf("OMNICONSENSUS Go: Peer %s voted for mutation %s: %v", peerID, mutationID, approve)
}
