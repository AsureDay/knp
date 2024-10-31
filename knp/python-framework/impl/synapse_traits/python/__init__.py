"""
@file __init__.py
@brief Import knp.synapse_traits.

@kaspersky_support Artiom N.
@license Apache 2.0 License.
@copyright © 2024 AO Kaspersky Lab
@date 28.10.2024.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""


from knp.synapse_traits._knp_python_framework_synapse_traits import (  # pylint: disable = no-name-in-module
    SynapticResourceSTDPDeltaSynapseParameters,
    SynapticResourceSTDPDeltaSynapseRule,
    DeltaSynapseParameters,
    OutputType,
)

__all__ = [
    'DeltaSynapseParameters',
    'SynapticResourceSTDPDeltaSynapseParameters',
    'SynapticResourceSTDPDeltaSynapseRule',
    'OutputType',
]
