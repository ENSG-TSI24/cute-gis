#include "geojsonfile.h"

GeoJsonFile::GeoJsonFile()
    : VectorData()
{}
GeoJsonFile::GeoJsonFile(const char* path)
     : VectorData(path)
{}
const char* GeoJsonFile::GetCRS() {
    OGRLayer* layer = this->data->GetLayer(0); // Récupérer la première couche
    OGRSpatialReference* spatialRef = layer->GetSpatialRef();
//    char* wkt = nullptr;
//    spatialRef->exportToWkt(&wkt);
//    std::cout << "CRS (WKT): " << wkt << std::endl;
    const char* code = spatialRef->GetAuthorityCode(nullptr);

    return code;
}

char** GeoJsonFile::GetMetadata() {
    char** metadata = this->data->GetMetadata();
    return metadata;

    // example pour la lecture
    /*
    std::cout << "Métadonnées du fichier : " << std::endl;
    if (metadata) {
        for (int i = 0; metadata[i] != nullptr; ++i) {
            std::cout << metadata[i] << std::endl;
        }
    }
    */
    GDALClose(this->data);
}

void GeoJsonFile::PrintGeoJsonAttributes() {

    if (this->GetDATA() == nullptr) {
        std::cerr << "Impossible d'ouvrir le fichier GeoJSON." << std::endl;
        return;
    }

    // Obtenir la première couche du GeoJSON (souvent une seule couche)
    OGRLayer* poLayer = this->GetDATA()->GetLayer(0);
    if (poLayer == nullptr) {
        std::cerr << "Aucune couche trouvée dans le fichier GeoJSON." << std::endl;
        GDALClose(this->GetDATA());
        return;
    }

    // Obtenir la définition de la couche (les métadonnées de la couche)
    OGRFeatureDefn* poFDefn = poLayer->GetLayerDefn();
    int nFieldCount = poFDefn->GetFieldCount();

    // Afficher les noms des attributs
    std::cout << "Attributs disponibles dans le fichier GeoJSON :" << std::endl;
    for (int i = 0; i < nFieldCount; i++) {
        OGRFieldDefn* poFieldDefn = poFDefn->GetFieldDefn(i);
        std::cout << "Attribut " << i + 1 << ": " << poFieldDefn->GetNameRef() << std::endl;
    }

    // Libérer les ressources
    GDALClose(this->data);
}

/*
    // Parcourir les entités (features) du fichier GeoJSON
    OGRFeature* feature;
    layer->ResetReading(); // Réinitialiser la lecture de la couche
    while ((feature = layer->GetNextFeature()) != nullptr) {
        // Récupérer les propriétés de la feature
        std::cout << "Propriétés de la feature :" << std::endl;
        OGRFieldDefn* fieldDefn;
        for (int i = 0; i < feature->GetFieldCount(); ++i) {
            fieldDefn = feature->GetFieldDefnRef(i);
            const char* fieldName = fieldDefn->GetNameRef();
            const char* fieldValue = feature->GetFieldAsString(i);
            std::cout << fieldName << ": " << fieldValue << std::endl;
        }

        // Récupérer la géométrie de la feature
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (geometry) {
            std::cout << "Géométrie (WKT) : " << std::endl;
            char* wkt = nullptr;
            geometry->exportToWkt(&wkt);
            std::cout << wkt << std::endl;
            CPLFree(wkt);
        }

        // Libérer la mémoire utilisée par la feature
        OGRFeature::DestroyFeature(feature);
    }

    // Fermer le dataset
    GDALClose(dataset);

 */
