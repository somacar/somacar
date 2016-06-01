from django.http import JsonResponse
from django.shortcuts import render_to_response
from geoposition import Geoposition

from scar.models import *


def img_view(request):
    return render_to_response('image.html', {"menu_img": True})


def info_view(request):
    return render_to_response('info.html', {"menu_info": True})


def loc_create(request, lat, lon):
    Location.objects.create(pos=Geoposition(lat, lon))
    data = {'success': True}
    return JsonResponse(data)


def map_view(request):
    loc = Location.objects.all()
    return render_to_response('map.html', {"menu_map": True, "loc": loc})
