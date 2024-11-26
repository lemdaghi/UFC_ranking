import requests
from bs4 import BeautifulSoup
import pandas as pd

# URL of the page to scrap
url = "http://www.ufcstats.com/statistics/events/completed?page=all"

# Send an HTTP GET request
response = requests.get(url)
soup = BeautifulSoup(response.text, 'html.parser')

# Find all the links of UFC events
event_links = []
tbody = soup.find('tbody')  # Locate the item <tbody>
if tbody:
    links = tbody.find_all('a', href=True)  # Find all the <a> with href
    for link in links:
        if "http://www.ufcstats.com/event-details" in link['href']:
            event_links.append(link['href'])

print(f"Number of event links recovered : {len(event_links)}")

def scrape_event_details(event_url):
    response = requests.get(event_url)
    soup = BeautifulSoup(response.text, 'html.parser')

    # Find the <tbody> containing fights
    tbody = soup.find('tbody', class_='b-fight-details__table-body')

    # Initialize a list to save fights
    fights = []

    # Browse each line of the fight table
    for row in tbody.find_all('tr', class_='b-fight-details__table-row'):
        # Extract columns
        columns = row.find_all('td')

        # Recover important information
        fight_details = {
            "link": row.get('data-link'),  # Link to fight details
            "fighter_1": columns[1].find_all('p')[0].text.strip(),  # 1st fight
            "fighter_2": columns[1].find_all('p')[1].text.strip(),  # 2nd fight
            "method": columns[7].find('p').text.strip(),  # Winning Method
            "round": columns[8].find('p').text.strip(),  # Round
            "time": columns[9].find('p').text.strip(),  # Time
            "weight_class": columns[6].find('p').text.strip(),  # wieght class
        }

        # Add fight information to the list
        fights.append(fight_details)
    return fights


# # Exemple of use
# for event_link in event_links[:5]:  # Limiter pour des tests
#     print(f"Scraping l'événement : {event_link}")
#     fights = scrape_event_details(event_link)
#     print(fights)


import pandas as pd

all_fights = []

for event_link in event_links:
    print(f"Scraping : {event_link}")
    event_fights = scrape_event_details(event_link)
    all_fights.extend(event_fights)

# Save in a CSV file
df = pd.DataFrame(all_fights)
df.to_csv('ufc_fights.csv', index=False)
print("Données sauvegardées dans ufc_fights.csv")
