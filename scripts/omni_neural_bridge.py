import asyncio
import json

try:
    import websockets
except ImportError:
    print("ERROR: Please install websockets: pip install websockets")
    exit(1)

async def omni_neural_bridge():
    uri = "ws://localhost:8080"
    print(f"OMNINEURAL BRIDGE: Attempting connection to OS Kernel at {uri}...")

    try:
        async with websockets.connect(uri) as websocket:
            print("OMNINEURAL BRIDGE: Connected! Awaiting Visual DOM Stream...")
            
            # Start an asynchronous task to read from the OS
            async def receive_dom():
                while True:
                    try:
                        message = await websocket.recv()
                        dom_data = json.loads(message)
                        print("\n[OS DOM SNAPSHOT RECEIVED]")
                        # Here is where you would pipe the JSON into Gemini or Claude
                        # e.g., prompt = f"Analyze this OS DOM and click the submit button: {dom_data}"
                        print(f"Widgets Active: {len(dom_data.get('children', []))}")
                    except websockets.exceptions.ConnectionClosed:
                        print("Connection closed by OS.")
                        break
            
            # Start an asynchronous task to send commands to the OS
            async def send_commands():
                while True:
                    # In a real LLM setup, the AI would generate these JSON command strings natively
                    cmd = await asyncio.to_thread(input, "\nEnter LLM JSON Command (e.g. {\"targetId\": \"Btn_1\", \"action\": \"click\"}): ")
                    if cmd:
                        try:
                            # Validate JSON before sending
                            json.loads(cmd)
                            await websocket.send(cmd)
                            print(f"-> Dispatched AI Action to OS Kernel: {cmd}")
                        except json.JSONDecodeError:
                            print("ERROR: Invalid JSON command.")

            await asyncio.gather(receive_dom(), send_commands())

    except ConnectionRefusedError:
        print("ERROR: Connection Refused. Make sure the BobUI native executable is running and OmniNeuralEngine is streaming.")

if __name__ == "__main__":
    asyncio.run(omni_neural_bridge())
