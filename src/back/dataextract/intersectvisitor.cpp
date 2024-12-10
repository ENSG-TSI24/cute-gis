#include "intersectvisitor.h"

IntersectVisitor::IntersectVisitor() {
    // nothing to instantiate at the moment
}

IntersectVisitor::~IntersectVisitor() {
}

// GDALDataset* IntersectVisitor::visitSHP(GDALDataset* shpDataset, GDALDataset* shp_intersectedDataset) {
//     return nullptr;
    
// }

// GDALDataset* IntersectVisitor::visitGeojson(GDALDataset* geoJsonDataset, GDALDataset* geoJson_intersectedDataset) {
    
//     OGRLayer* geoJsonLayer = geoJsonDataset->GetLayer(0);
//     OGRLayer* intersectedLayer = geoJson_intersectedDataset->GetLayer(0);

//     if (geoJsonLayer == nullptr || intersectedLayer == nullptr) {
//         return nullptr;
//     }

//     GDALDataset* outputDataset = (GDALDataset*) GDALOpen("../data/intersected_output.geojson", GA_Update);
    
//     if (outputDataset == nullptr) {
//         return nullptr;
//     }

// OGRLayer* outputLayer = outputDataset->CreateLayer("intersected_layer", nullptr, wkbUnknown, nullptr);
//     if (outputLayer == nullptr) {
//         GDALClose(outputDataset);
//         return nullptr;
//     }

//     geoJsonLayer->ResetReading();
//     OGRFeature* geoJsonFeature;
//     while ((geoJsonFeature = geoJsonLayer->GetNextFeature()) != nullptr) {
//         intersectedLayer->ResetReading();
//         OGRFeature* intersectedFeature;
//         while ((intersectedFeature = intersectedLayer->GetNextFeature()) != nullptr) {
//             if (geoJsonFeature->GetGeometryRef()->Intersects(intersectedFeature->GetGeometryRef())) {
//                 OGRFeature* newFeature = OGRFeature::CreateFeature(outputLayer->GetLayerDefn());
//                 newFeature->SetGeometry(geoJsonFeature->GetGeometryRef()->Intersection(intersectedFeature->GetGeometryRef()));
//                 outputLayer->CreateFeature(newFeature);
//                 OGRFeature::DestroyFeature(newFeature);
//             }
//             OGRFeature::DestroyFeature(intersectedFeature);
//         }
//         OGRFeature::DestroyFeature(geoJsonFeature);
//     }

//     return outputDataset;
// }