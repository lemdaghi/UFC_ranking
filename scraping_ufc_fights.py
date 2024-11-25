import requests
from bs4 import BeautifulSoup
import pandas as pd

# URL de la page à scraper
url = "http://www.ufcstats.com/statistics/events/completed?page=all"  # Exemple, modifiez selon vos besoins

# Envoyer une requête HTTP GET
response = requests.get(url)
soup = BeautifulSoup(response.text, 'html.parser')

# Trouver tous les liens d'événements
event_links = []
tbody = soup.find('tbody')  # Localiser l'élément <tbody>
if tbody:
    links = tbody.find_all('a', href=True)  # Trouver tous les <a> avec href
    for link in links:
        if "http://www.ufcstats.com/event-details" in link['href']:
            event_links.append(link['href'])

print(f"Liens des événements récupérés : {len(event_links)}")

# # Extraction des données    
# fighters = []
# for fighter in soup.find_all('div', class_='athlete-detail'):
#     name = fighter.find('a', class_='name').text.strip()
#     wins = int(fighter.find('span', class_='wins').text.strip())
#     losses = int(fighter.find('span', class_='losses').text.strip())
#     method = fighter.find('div', class_='method').text.strip()  # Hypothèse
    
#     fighters.append({
#         'Name': name,
#         'Wins': wins,
#         'Losses': losses,
#         'Method': method
#     })

# # Exporter les données dans un fichier CSV
# df = pd.DataFrame(fighters)
# df.to_csv('ufc_fighters.csv', index=False)

def scrape_event_details(event_url):
    response = requests.get(event_url)
    soup = BeautifulSoup(response.text, 'html.parser')

    # Trouver le <tbody> contenant les combats
    tbody = soup.find('tbody', class_='b-fight-details__table-body')

    # Initialiser une liste pour stocker les combats
    fights = []

    # Parcourir chaque ligne du tableau des combats
    for row in tbody.find_all('tr', class_='b-fight-details__table-row'):
        # Extraire les colonnes
        columns = row.find_all('td')

        # Récupérer les informations importantes
        fight_details = {
            "link": row.get('data-link'),  # Lien vers les détails du combat
            "fighter_1": columns[1].find_all('p')[0].text.strip(),  # Premier combattant
            "fighter_2": columns[1].find_all('p')[1].text.strip(),  # Deuxième combattant
            "method": columns[7].find('p').text.strip(),  # Méthode de victoire
            "round": columns[8].find('p').text.strip(),  # Round
            "time": columns[9].find('p').text.strip(),  # Temps
            "weight_class": columns[6].find('p').text.strip(),  # Catégorie de poids
        }

        # Ajouter les informations du combat à la liste
        fights.append(fight_details)
    return fights



    # # Récupérer les combats de l'événement
    # fights = []
    # fight_rows = soup.find_all('tr', class_='b-fight-details__table-row')
    # for row in fight_rows:
    #     fighter_data = row.find_all('td', class_='b-fight-details__table-col')
    #     if fighter_data:
    #         try:
    #             fighter_1 = fighter_data[1].text.strip()  # Premier combattant
    #             fighter_2 = fighter_data[2].text.strip()  # Deuxième combattant
    #             result = fighter_data[0].text.strip()    # Résultat
    #             method = fighter_data[3].text.strip()   # Méthode
    #             fights.append({
    #                 'Fighter 1': fighter_1,
    #                 'Fighter 2': fighter_2,
    #                 'Result': result,
    #                 'Method': method
    #             })
    #         except IndexError:
    #             continue

    # return fights

# Exemple d'utilisation
for event_link in event_links[:5]:  # Limiter pour des tests
    print(f"Scraping l'événement : {event_link}")
    fights = scrape_event_details(event_link)
    print(fights)




import pandas as pd

all_fights = []

for event_link in event_links:
    print(f"Scraping : {event_link}")
    event_fights = scrape_event_details(event_link)
    all_fights.extend(event_fights)

# Sauvegarder dans un fichier CSV
df = pd.DataFrame(all_fights)
df.to_csv('ufc_fights.csv', index=False)
print("Données sauvegardées dans ufc_fights.csv")
