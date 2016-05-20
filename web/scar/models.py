from django.db import models
from geoposition.fields import GeopositionField


class Location(models.Model):
    pos = GeopositionField()

    def __str__(self):
        return self.pos
