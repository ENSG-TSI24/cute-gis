# GIS Application Project - Branch Dev

### Commun à toute les API de l'iGN

```C++
	https://data.geopf.fr/[chemin vers le service]?SERVICE=[nom du service]&VERSION=[version]&REQUEST=[type de requête]&PARAM1&PARMA2&...
```
Où respectivement : 
- chemin vers le service = chemin dans les dossiers du server geopf, ex : wms-v/ows ou wmts
- nom du service = nom du service en MAJ, ex : WMTS
- type de requête =  au choix : 
	 - GetCapabilities pour obtenir les métadonnées du service
- les types de requêtes suivantes changent selon le service appelés :
   	 - GetTile pour obtenir une tuile
  	 - GetFeatureInfo pour obtenir les métadonnées d'une tuile
  	 - GetMap pour obtenir une carte
  	 - GetFeatures (hits) nombre d'objets dans la demande ou (result) les objets reçus
  	  

### liens vers decriptif des différentes API :

##### Flux : 
https://geoservices.ign.fr/documentation/services/services-geoplateforme/diffusion#70066

##### Géocodage :
https://geoservices.ign.fr/documentation/services/services-geoplateforme/geocodage

### WFS Géoportail :
https://apicarto.ign.fr/api/doc/wfs-geoportail

##### Descriptif API cityGML grand Lyon :

https://data.grandlyon.com/portail/fr/jeux-de-donnees/maquettes-3d-texturees-2018-communes-metropole-lyon/api
