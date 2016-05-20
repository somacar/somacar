from django.db import models
from geoposition.fields import GeopositionField


class Location(models.Model):
    pos = GeopositionField()
    dt = models.DateField(auto_now=True)

    def __str__(self):
        return self.dt
