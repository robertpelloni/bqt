package net

import (
	"fmt"
	"log"
	"net"
	"sync"
	"time"
)

type DiscoveryBeacon struct {
	Active bool

	mu           sync.Mutex
	broadcasting bool
	conn         *net.UDPConn
}

var (
	beaconInstance *DiscoveryBeacon
	beaconOnce     sync.Once
)

// GetDiscoveryBeacon returns the shared beacon. Previously it returned a new
// instance per call, so Active/broadcast state could never be observed or
// stopped coherently.
func GetDiscoveryBeacon() *DiscoveryBeacon {
	beaconOnce.Do(func() { beaconInstance = &DiscoveryBeacon{Active: true} })
	return beaconInstance
}

// IsBroadcasting reports whether the broadcast goroutine is running.
func (db *DiscoveryBeacon) IsBroadcasting() bool {
	if db == nil {
		return false
	}
	db.mu.Lock()
	defer db.mu.Unlock()
	return db.broadcasting
}

// StartBroadcasting pings the network so other BQt nodes can find us.
// Idempotent: repeat calls while broadcasting are no-ops. A failed dial is
// logged and leaves the beacon stopped instead of panicking on a nil conn.
func (db *DiscoveryBeacon) StartBroadcasting(port int) {
	db.mu.Lock()
	if db.broadcasting {
		db.mu.Unlock()
		return
	}
	addr, err := net.ResolveUDPAddr("udp", "255.255.255.255:9999")
	if err != nil {
		db.mu.Unlock()
		log.Printf("OMNICLOUD Go: discovery resolve failed: %v", err)
		return
	}
	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil || conn == nil {
		db.mu.Unlock()
		log.Printf("OMNICLOUD Go: discovery dial failed: %v", err)
		return
	}
	db.conn = conn
	db.broadcasting = true
	db.Active = true
	db.mu.Unlock()

	go func() {
		for db.IsBroadcasting() {
			conn.Write([]byte(fmt.Sprintf("OMNI_NODE:%d", port)))
			time.Sleep(5 * time.Second)
		}
	}()
	log.Println("OMNICLOUD Go: Zero-Config Discovery Beacon started.")
}

// StopBroadcasting halts broadcasts and releases the socket.
func (db *DiscoveryBeacon) StopBroadcasting() {
	db.mu.Lock()
	defer db.mu.Unlock()
	db.broadcasting = false
	db.Active = false
	if db.conn != nil {
		db.conn.Close()
		db.conn = nil
	}
}

// ListenForPeers captures remote beacons and adds them to our Mesh.
func (db *DiscoveryBeacon) ListenForPeers() {
	addr, _ := net.ResolveUDPAddr("udp", ":9999")
	conn, err := net.ListenUDP("udp", addr)
	if err != nil || conn == nil {
		log.Printf("OMNICLOUD Go: discovery listen failed: %v", err)
		return
	}
	go func() {
		defer conn.Close()
		buf := make([]byte, 1024)
		for db.Active {
			n, remoteAddr, err := conn.ReadFromUDP(buf)
			if err != nil {
				return
			}
			log.Printf("OMNICLOUD Go: Discovered remote peer at %s -> %s", remoteAddr, string(buf[:n]))
		}
	}()
}
