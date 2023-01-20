from django.contrib import admin
from .models import Sensor, Amostra
# Register your models here.


class SensorAdmin(admin.ModelAdmin):
    list_display = ('nome', 'grupo')
    
class AmostraAdmin(admin.ModelAdmin):
    list_display = ('sensor', 'timestamp','valor','unidade')

admin.site.register(Sensor,SensorAdmin)
admin.site.register(Amostra,AmostraAdmin)