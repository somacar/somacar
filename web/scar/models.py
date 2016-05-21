from datetime import date

from django.db import models
from django.utils.timesince import timesince
from geoposition.fields import GeopositionField


class Location(models.Model):
    pos = GeopositionField()
    dt = models.DateTimeField(auto_now=True)

    def __str__(self):
        return str(self.dt)

    def get_time(self):
        if self.dt.date() == date.today():
            diff = timesince(self.dt)
            if diff == '0분':
                return '방금 전'
            return '%s 전' % timesince(self.dt).split(',')[0]
        else:
            return self.dt.date()
