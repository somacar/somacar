from django.shortcuts import render_to_response

from scar.models import *


def map_view(request):
    loc = Location.objects.all()
    return render_to_response('map.html', {"loc": loc})


def img_view(request):
    return render_to_response('image.html')
