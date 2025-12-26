import requests
import time

website_link = 'https://cpuoh2026.streamlit.app'

if __name__ == '__main__':
    while True:
        time.sleep(10)
        client = requests.session()
        req = client.get(website_link)
        print(req.text)
