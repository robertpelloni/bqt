import urllib.request
import json
import sys

def get_latest_release():
    url = "https://api.github.com/repos/robertpelloni/bobui/releases/latest"
    try:
        req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        with urllib.request.urlopen(req) as response:
            data = json.loads(response.read().decode())
            print(f"=====================================")
            print(f"🚀 OMNIUI LATEST RELEASE: {data['tag_name']}")
            print(f"=====================================")
            for asset in data.get('assets', []):
                print(f"⬇️ Download {asset['name']}:")
                print(f"   {asset['browser_download_url']}")
            print(f"=====================================")
    except Exception as e:
        print(f"Failed to fetch release data. Make sure the CI/CD pipeline has completed on GitHub: {e}")

if __name__ == "__main__":
    get_latest_release()
