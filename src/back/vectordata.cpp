#include "vectordata.h"
#include "src/back/datamanagment.h"
#include <cstddef>
#include <iostream>
#include <ogrsf_frmts.h>

VectorData::VectorData() : DataManagment() {}

VectorData::VectorData(const char* path) : DataManagment(path) {}


VectorData::~VectorData() {}

std::vector<std::pair<float, float>> VectorData::GetPoints(){
    std::vector<std::pair<float, float>> points;
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset *) GDALOpenEx(this->GetPath(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    // Accéder à la première couche
    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        throw std::runtime_error("Failed to get layer from GDALDataset.");
    }

    // Parcourir les entités de la couche
    OGRFeature* feature;
    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (!geometry) {
            OGRFeature::DestroyFeature(feature);
            continue;
        }

        // Traitement des géométries selon leur type
        switch (geometry->getGeometryType()) {
        case wkbPoint: {
            OGRPoint* point = geometry->toPoint();
            points.emplace_back(point->getX(), point->getY());
            break;
        }
        default:
            // Géométries non supportées
            break;
        }

        // Libérer la mémoire de l'entité
        OGRFeature::DestroyFeature(feature);
    }
    GDALClose(dataset);
    return points;
}

std::vector<std::vector<std::pair<float, float>>> VectorData::GetLineStrings(){
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset *) GDALOpenEx(this->GetPath(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    // Accéder à la première couche
    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        throw std::runtime_error("Failed to get layer from GDALDataset.");
    }

    // Parcourir les entités de la couche
    OGRFeature* feature;
    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (!geometry) {
            OGRFeature::DestroyFeature(feature);
            continue;
        }

        // Traitement des géométries selon leur type
        switch (geometry->getGeometryType()) {
        case wkbLineString: {
            OGRLineString* lineString = geometry->toLineString();
            std::vector<std::pair<float, float>> line;
            for (int i = 0; i < lineString->getNumPoints(); ++i) {
                line.emplace_back(lineString->getX(i), lineString->getY(i));
            }
            linestrings.push_back(line);
            break;
        }
        case wkbMultiLineString: {
            OGRMultiLineString* multiLineString = geometry->toMultiLineString();
            for (int i = 0; i < multiLineString->getNumGeometries(); ++i) {
                OGRLineString* lineString = (OGRLineString*)multiLineString->getGeometryRef(i);
                std::vector<std::pair<float, float>> line;
                for (int j = 0; j < lineString->getNumPoints(); ++j) {
                    line.emplace_back(lineString->getX(j), lineString->getY(j));
                }
                linestrings.push_back(line);
            }
            break;
        }
        default:
            // Géométries non supportées
            break;
        }

        // Libérer la mémoire de l'entité
        OGRFeature::DestroyFeature(feature);
    }
    GDALClose(dataset);
    return linestrings;
}

std::vector<std::vector<std::vector<std::tuple<float, float, float>>>> VectorData::GetPolygons(){
    std::vector<std::vector<std::vector<std::tuple<float, float, float>>>> polygons;
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset *) GDALOpenEx(this->GetPath(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    // Accéder à la première couche
    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        throw std::runtime_error("Failed to get layer from GDALDataset.");
    }

    // Parcourir les entités de la couche
    OGRFeature* feature;
    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (!geometry) {
            OGRFeature::DestroyFeature(feature);
            continue;
        }

        // Traitement des géométries selon leur type
        switch (geometry->getGeometryType()) {
        case wkbPolygon:
        case wkbPolygon25D: {
            OGRPolygon* polygon = geometry->toPolygon();
            std::vector<std::vector<std::tuple<float, float, float>>> polyRings;

            for (int i = 0; i < polygon->getNumInteriorRings() + 1; ++i) {
                const OGRLinearRing* ring = (i == 0) ? polygon->getExteriorRing() : polygon->getInteriorRing(i - 1);
                if (ring) {
                    std::vector<std::tuple<float, float, float>> ringPoints;
                    for (int j = 0; j < ring->getNumPoints(); ++j) {
                        ringPoints.emplace_back(ring->getX(j), ring->getY(j), ring->getZ(j));
                    }
                    polyRings.push_back(ringPoints);
                }
            }
            polygons.push_back(polyRings);
            break;
        }
        case wkbMultiPolygon:
        case wkbMultiPolygon25D: {
            OGRMultiPolygon* multiPolygon = geometry->toMultiPolygon();
            for (int i = 0; i < multiPolygon->getNumGeometries(); ++i) {
                OGRPolygon* polygon = (OGRPolygon*)multiPolygon->getGeometryRef(i);
                std::vector<std::vector<std::tuple<float, float, float>>> polyRings;

                for (int j = 0; j < polygon->getNumInteriorRings() + 1; ++j) {
                    const OGRLinearRing* ring = (j == 0) ? polygon->getExteriorRing() : polygon->getInteriorRing(j - 1);
                    if (ring) {
                        std::vector<std::tuple<float, float, float>> ringPoints;
                        for (int k = 0; k < ring->getNumPoints(); ++k) {
                            ringPoints.emplace_back(ring->getX(k), ring->getY(k), ring->getZ(k));
                        }
                        polyRings.push_back(ringPoints);
                    }
                }
                polygons.push_back(polyRings);
            }
            break;
        }
        default:
            // Géométries non supportées
            break;
        }

        // Libérer la mémoire de l'entité
        OGRFeature::DestroyFeature(feature);
    }
    GDALClose(dataset);
    return polygons;
}

std::vector<std::vector<std::vector<std::pair<float, float>>>> VectorData::Get2DPolygons(){
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset *) GDALOpenEx(this->GetPath(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        throw std::runtime_error("Failed to get layer from GDALDataset.");
    }

    OGRFeature* feature;
    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (!geometry) {
            OGRFeature::DestroyFeature(feature);
            continue;
        }
        switch (geometry->getGeometryType()) {
        case wkbPolygon: {
            OGRPolygon* polygon = geometry->toPolygon();
            std::vector<std::vector<std::pair<float, float>>> polyRings;

            for (int i = 0; i < polygon->getNumInteriorRings() + 1; ++i) {
                const OGRLinearRing* ring = (i == 0) ? polygon->getExteriorRing() : polygon->getInteriorRing(i - 1);
                if (ring) {
                    std::vector<std::pair<float, float>> ringPoints;
                    for (int j = 0; j < ring->getNumPoints(); ++j) {
                        ringPoints.emplace_back(ring->getX(j), ring->getY(j));
                    }
                    polyRings.push_back(ringPoints);
                }
            }
            polygons.push_back(polyRings);
            break;
        }
        case wkbMultiPolygon: {
            OGRMultiPolygon* multiPolygon = geometry->toMultiPolygon();
            for (int i = 0; i < multiPolygon->getNumGeometries(); ++i) {
                OGRPolygon* polygon = (OGRPolygon*)multiPolygon->getGeometryRef(i);
                std::vector<std::vector<std::pair<float, float>>> polyRings;

                for (int j = 0; j < polygon->getNumInteriorRings() + 1; ++j) {
                    const OGRLinearRing* ring = (j == 0) ? polygon->getExteriorRing() : polygon->getInteriorRing(j - 1);
                    if (ring) {
                        std::vector<std::pair<float, float>> ringPoints;
                        for (int k = 0; k < ring->getNumPoints(); ++k) {
                            ringPoints.emplace_back(ring->getX(k), ring->getY(k));
                        }
                        polyRings.push_back(ringPoints);
                    }
                }
                polygons.push_back(polyRings);
            }
            break;
        }
        default:
            break;
        }

        OGRFeature::DestroyFeature(feature);
    }
    GDALClose(dataset);
    return polygons;
}

std::vector<std::string> VectorData::GetAttributName() {
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset *) GDALOpenEx(this->GetPath(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    std::vector<std::string> header_name;

    OGRLayer* layer = dataset->GetLayer(0); //On considère que chaque fichier contient une seule couche?
    int field_count = layer->GetLayerDefn()->GetFieldCount(); //Nombre d'attributs dans la couche

    for (int i = 0; i < field_count; ++i) {
        const char *field_name = layer->GetLayerDefn()->GetFieldDefn(i)->GetNameRef();//On récupère le nom de chaque champs et on le passe en char
        //std::cout << field_name << std::endl;
        header_name.emplace_back(field_name);
    }
    GDALClose(dataset);
    return header_name;
}

std::vector<std::vector<std::string>> VectorData::GetAllAttributData() {
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset *) GDALOpenEx(this->GetPath(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    std::vector<std::vector<std::string>> row;
    std::vector<std::string> column;

    OGRLayer* poLayer = dataset->GetLayer(0); //On considère que chaque fichier contient une seule couche?
    for( auto& poFeature: poLayer )//On parcours les entités de la couche
    {
        for( auto&& oField: *poFeature )//On parcours chaque champs pour toutes les entités
        {
        if( oField.IsNull() )
        {
            column.emplace_back("");
            continue;
        }
            column.emplace_back(oField.GetAsString());
            //std::cout << oField.GetAsString() << std::endl;
        }
        row.emplace_back(std::move(column));
    }
    GDALClose(dataset);
    return row;
}



std::vector<std::string> VectorData::GetAttributeDataByHeader(const char* fieldName) {
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset *) GDALOpenEx(this->GetPath(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    std::vector<std::string> data_field;

    OGRLayer* poLayer = dataset->GetLayer(0); //On considère que chaque fichier contient une seule couche?
    for( auto& poFeature: poLayer )
    {
        data_field.emplace_back(poFeature->GetFieldAsString(fieldName));//Pour chaque entité, on récupère la valeur du champs fieldname
        //std::cout << poFeature->GetFieldAsString(fieldName) << std::endl;
    }
    GDALClose(dataset);
    return data_field;
}

std::vector<std::vector<std::string>> VectorData::GetAttributeDataById(int id) {
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset *) GDALOpenEx(this->GetPath(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    std::vector<std::vector<std::string>> row;
    std::vector<std::string> column;

    OGRLayer* poLayer = dataset->GetLayer(0);
    OGRFeature *poFeature = poLayer->GetFeature(id);//Entité unique placé à la ligne id
        for( auto&& oField: *poFeature ) {
            if( oField.IsNull() )
            {
                column.emplace_back("");
            }
            else
            {
                column.emplace_back(oField.GetAsString());
                //std::cout << oField.GetAsString() << std::endl;
            }
            row.emplace_back(column);
        }

    GDALClose(dataset);
    return row;
}
